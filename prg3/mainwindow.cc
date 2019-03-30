// The main program (provided by the course), TIE-20100/TIE-20106
//
// DO ****NOT**** EDIT THIS FILE!
// (Preferably do not edit this even temporarily. And if you still decide to do so
//  (for debugging, for example), DO NOT commit any changes to git, but revert all
//  changes later. Otherwise you won't be able to get any updates/fixes to this
//  file from git!)

#include "mainprogram.hh"

// Qt generated main window code

#ifdef GRAPHICAL_GUI
#include <QCoreApplication>
#include <QFileDialog>
#include <QDir>
#include <QFont>
#include <QGraphicsItem>
#include <QColor>
#include <QBrush>
#include <QPen>
#include <QGraphicsItem>
#include <QVariant>

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;

#include <cassert>

#include "mainwindow.hh"
#include "ui_mainwindow.h"

// Needed to be able to store TownID in QVariant (in QGraphicsItem)
Q_DECLARE_METATYPE(TownID)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mainprg_.setui(this);

    // Execute line
    connect(ui->execute_button, &QPushButton::pressed, this, &MainWindow::execute_line);

    // Line edit
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &MainWindow::execute_line);

    // File selection
    connect(ui->file_button, &QPushButton::pressed, this, &MainWindow::select_file);

    // Command selection
    // !!!!! Sort commands in alphabetical order (should not be done here, but is)
    std::sort(mainprg_.cmds_.begin(), mainprg_.cmds_.end(), [](auto& l, auto& r){ return l.cmd < r.cmd; });
    for (auto& cmd : mainprg_.cmds_)
    {
        ui->cmd_select->addItem(QString::fromStdString(cmd.cmd));
    }
    connect(ui->cmd_select, static_cast<void(QComboBox::*)(int)>(&QComboBox::activated), this, &MainWindow::cmd_selected);

    // Number selection
    for (auto i = 0; i <= 20; ++i)
    {
        ui->number_select->addItem(QString("%1").arg(i));
    }
    connect(ui->number_select, static_cast<void(QComboBox::*)(QString const&)>(&QComboBox::activated), this, &MainWindow::number_selected);

    // Output box
    QFont monofont("Monospace");
    monofont.setStyleHint(QFont::Monospace);
    ui->output->setFont(monofont);

    // Initialize graphics scene & view
    gscene_ = new QGraphicsScene(this);
    ui->graphics_view->setScene(gscene_);
//    for (auto x=0u; x<10; ++x)
//    {
//        for (auto y=0u; y<10; ++y)
//        {
//            QGraphicsItem* p = gscene_->addRect(10*x, 10*y, 10, 10, QPen(Qt::NoPen), QBrush(Qt::red));
//            p->setFlag(QGraphicsItem::ItemIsSelectable);
//            gscene_->addLine(10*x, 10*y, 10*(x+1), 10*y);
//            gscene_->addLine(10*x, 10*y, 10*x, 10*(y+1));
//            if (x == 9) { gscene_->addLine(10*(x+1), 10*y, 10*(x+1), 10*(y+1)); }
//            if (y == 9) { gscene_->addLine(10*x, 10*(y+1), 10*(x+1), 10*(y+1)); }
//        }
//    }
    ui->graphics_view->resetTransform();

    // Selecting graphics items by mouse
    connect(gscene_, &QGraphicsScene::selectionChanged, this, &MainWindow::scene_selection_change);
//    connect(this, &MainProgram::signal_clear_selection, this, &MainProgram::clear_selection);

    // Zoom slider changes graphics view scale
    connect(ui->zoom_plus, &QToolButton::clicked, [this](){ this->ui->graphics_view->scale(1.1, 1.1); });
    connect(ui->zoom_minus, &QToolButton::clicked, [this](){ this->ui->graphics_view->scale(1/1.1, 1/1.1); });
    connect(ui->zoom_1, &QToolButton::clicked, [this](){ this->ui->graphics_view->resetTransform(); });
    connect(ui->zoom_fit, &QToolButton::clicked, this, &MainWindow::fit_view);

    // Changing roads/vassals checkboxes updates view
    connect(ui->vassals_checkbox, &QCheckBox::clicked, this, &MainWindow::update_view);
    connect(ui->roads_checkbox, &QCheckBox::clicked, this, &MainWindow::update_view);
    connect(ui->towns_checkbox, &QCheckBox::clicked, this, &MainWindow::update_view);

    // Changing font scale updates view
    connect(ui->fontscale, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &MainWindow::update_view);

    // Clear input button
    connect(ui->clear_input_button, &QPushButton::clicked, this, &MainWindow::clear_input_line);

    // Stop button
    connect(ui->stop_button, &QPushButton::clicked, [this](){ this->stop_pressed_ = true; });

    clear_input_line();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_view()
{
//    ui->output->appendPlainText("Update view:");

    gscene_->clear();
    auto fontscale = ui->fontscale->value();

    auto towns = mainprg_.ds_.all_towns();
    for (auto& townid : towns)
    {
        auto name = mainprg_.ds_.get_name(townid);
//        if (name.empty()) { continue; } // Ignore crossroads (i.e. towns with empty name)
        QColor color = Qt::blue;

        auto& prev_result = mainprg_.prev_result.second;
        auto res_index = prev_result.size(); // Default: not in result
        auto res_place = std::find(prev_result.begin(), prev_result.end(), townid);
        res_index = res_place - prev_result.begin();

        int x, y;
        std::tie(x,y) = mainprg_.ds_.get_coordinates(townid);
        auto groupitem = gscene_->createItemGroup({});
        groupitem->setPos(20*x, -20*y);
        groupitem->setFlag(QGraphicsItem::ItemIsSelectable);
        groupitem->setData(0, QVariant::fromValue(townid));

        if (mainprg_.prev_result.first == MainProgram::ResultType::LIST)
        {
            if (res_index < prev_result.size())
            {
                if (prev_result.size() > 1) { name.insert(0, MainProgram::convert_to_string(res_index+1)+". "); }
                color = Qt::red;
            }
        }

        if (!name.empty())
        {
            auto dotitem = gscene_->addEllipse(20*x-4*fontscale, -20*y-4*fontscale, 8*fontscale, 8*fontscale, QPen(), QBrush(color));
            groupitem->addToGroup(dotitem);
            //        dotitem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
            //        dotitem->setData(0, QVariant::fromValue(town));
        }
        else
        {
            auto dotitem = gscene_->addEllipse(20*x-0.5*fontscale, -20*y-0.5*fontscale, fontscale, fontscale, QPen(), QBrush(color));
            groupitem->addToGroup(dotitem);
        }

        // Draw towns
        if (ui->towns_checkbox->isChecked() && !name.empty())
        {
            auto textitem = gscene_->addSimpleText(QString::fromStdString(name));
            textitem->setPos(20*x-4*fontscale, -20*y-19*fontscale);
            textitem->setBrush(QBrush(color));
            auto font = textitem->font();
            font.setPointSizeF(font.pointSizeF()*fontscale);
            textitem->setFont(font);
    //        textitem->setTransformOriginPoint(20*town->x, -20*town->y);
            groupitem->addToGroup(textitem);
    //        textitem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        }

        // Draw vassal relations
        if (ui->vassals_checkbox->isChecked())
        {
            auto vassals = mainprg_.ds_.get_vassals(townid);
            for (auto& vassalid : vassals)
            {
                int vx, vy;
                std::tie(vx,vy) = mainprg_.ds_.get_coordinates(vassalid);

                QColor linecolor = Qt::gray;
                int zvalue = -1;

                if (mainprg_.prev_result.first == MainProgram::ResultType::HIERARCHY)
                {
                    if (res_index < prev_result.size())
                    {
                        if ((res_index > 0 && prev_result[res_index-1] == vassalid) ||
                            (res_index+1 < prev_result.size() && prev_result[res_index+1] == vassalid))
                        {
                            linecolor = Qt::red;
                            zvalue = 10;
                        }
                    }
                }

    //            auto pen = QPen(linecolor);
    //            pen.setWidth(0); // "Cosmetic" pen
    //            auto lineitem = gscene_->addLine(20*x+4, -20*y+4, 20*vx+4, -20*vy+4, pen);
                auto pen = QPen(linecolor);
                pen.setWidth(0); // "Cosmetic" pen

                double const PI  = 3.141592653589793238463;
                auto arrowSize = 10*fontscale;
                QLineF line(QPointF(20*x, -20*y), QPointF(20*vx, -20*vy));

                double angle = std::atan2(-line.dy(), line.dx());

                QPointF arrowP1 = line.p1() + QPointF(sin(angle + PI / 3) * arrowSize,
                                                cos(angle + PI / 3) * arrowSize);
                QPointF arrowP2 = line.p1() + QPointF(sin(angle + PI - PI / 3) * arrowSize,
                                                cos(angle + PI - PI / 3) * arrowSize);

                QPolygonF arrowHead;
                arrowHead << line.p1() << arrowP1 << arrowP2;
                auto lineitem = gscene_->addLine(line, pen);
                lineitem->setZValue(zvalue);
                auto headitem = gscene_->addPolygon(arrowHead, pen, QBrush(pen.color()));
                headitem->setZValue(zvalue);
            }
        }

        // Draw roads
        if (ui->roads_checkbox->isChecked())
        {
            auto roads = mainprg_.ds_.get_roads_from(townid);
            for (auto& roadid : roads)
            {
                int rx, ry;
                std::tie(rx,ry) = mainprg_.ds_.get_coordinates(roadid);

                QColor linecolor = Qt::black;
                int zvalue = -1;

                if (mainprg_.prev_result.first == MainProgram::ResultType::PATH ||
                    mainprg_.prev_result.first == MainProgram::ResultType::CYCLE)
                {
                    if (res_index < prev_result.size())
                    {
                        if ((res_index > 0 && prev_result[res_index-1] == roadid) ||
                            (res_index+1 < prev_result.size() && prev_result[res_index+1] == roadid))
                        {
                            linecolor = Qt::red;
                            zvalue = 10;
                        }
                    }
                }

                auto pen = QPen(linecolor);
                pen.setWidth(0); // "Cosmetic" pen
                auto lineitem = gscene_->addLine(20*x, -20*y, 20*rx, -20*ry, pen);
                lineitem->setZValue(zvalue);
    //            auto pen = QPen(linecolor);
    //            pen.setWidth(0); // "Cosmetic" pen
    //            auto arrow = new Arrow(QPointF(20*x+4, -20*y+4), QPointF(20*rx+4, -20*ry+4));
    //            arrow->setPen(pen);
    //            gscene_->addItem(arrow);
    //            arrow->setZValue(-1);
            }
        }
    }
}

void MainWindow::output_text(ostringstream& output)
{
    string outstr = output.str();
    if (!outstr.empty())
    {
        if (outstr.back() == '\n') { outstr.pop_back(); } // Remove trailing newline
        ui->output->appendPlainText(QString::fromStdString(outstr));
        ui->output->ensureCursorVisible();
        ui->output->repaint();
    }

    output.str(""); // Clear the stream, because it has already been output
}

void MainWindow::output_text_end()
{
    ui->output->moveCursor(QTextCursor::End);
    ui->output->ensureCursorVisible();
    ui->output->repaint();
}

bool MainWindow::check_stop_pressed() const
{
    QCoreApplication::processEvents();
    return stop_pressed_;
}

void MainWindow::execute_line()
{
    auto line = ui->lineEdit->text();
    clear_input_line();
    ui->output->appendPlainText(QString::fromStdString(MainProgram::PROMPT)+line);

    ui->execute_button->setEnabled(false);
    ui->stop_button->setEnabled(true);
    stop_pressed_ = false;

    ostringstream output;
    bool cont = mainprg_.command_parse_line(line.toStdString(), output);
    ui->lineEdit->clear();
    output_text(output);
    output_text_end();

    ui->stop_button->setEnabled(false);
    ui->execute_button->setEnabled(true);
    stop_pressed_ = false;

    ui->lineEdit->setFocus();

//    if (mainprg_.view_dirty)
//    {
//        update_view();
//        mainprg_.view_dirty = false;
//    }
    update_view();

    if (!cont)
    {
        close();
    }
}

void MainWindow::cmd_selected(int idx)
{
    ui->lineEdit->insert(QString::fromStdString(mainprg_.cmds_[idx].cmd+" "));
    ui->cmd_info_text->setText(QString::fromStdString(mainprg_.cmds_[idx].cmd+" "+mainprg_.cmds_[idx].info));

    ui->lineEdit->setFocus();
}

void MainWindow::number_selected(QString number)
{
    ui->lineEdit->insert(number+" ");

    ui->lineEdit->setFocus();
}

void MainWindow::select_file()
{
    QFileDialog dialog(this, "Select file", "", "Command list (*.txt)");
    dialog.setFileMode(QFileDialog::ExistingFiles);
    if (dialog.exec())
    {
        auto filenames = dialog.selectedFiles();
        for (auto& i : filenames)
        {
            auto filename = QDir("").relativeFilePath(i);
            ui->lineEdit->insert("\""+filename+"\" ");
        }
    }

    ui->lineEdit->setFocus();
}

void MainWindow::clear_input_line()
{
    ui->cmd_info_text->clear();
    ui->lineEdit->clear();
    ui->lineEdit->setFocus();
}

void MainWindow::fit_view()
{
    ui->graphics_view->fitInView(gscene_->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::scene_selection_change()
{
    auto items = gscene_->selectedItems();
    if (!items.empty())
    {
        for (auto i : items)
        {
            auto townid = i->data(0).value<TownID>();
            assert(townid!=NO_ID && "NO_ID as town in graphics view");
            if (!selection_clear_in_progress)
            {
                ostringstream output;
                output << "*click* ";
                mainprg_.print_town(townid, output);
                output_text(output);
                output_text_end();
                ui->lineEdit->insert(QString::fromStdString(townid+" "));
            }
            i->setSelected(false);
            selection_clear_in_progress = !selection_clear_in_progress;
        }
    }
}

void MainWindow::clear_selection()
{
    gscene_->clearSelection();
}



// Originally in main.cc
#include <QApplication>

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        auto status = a.exec();
        std::cerr << "Program ended normally." << std::endl;
        return status;
    }
    else
    {
        return MainProgram::mainprogram(argc, argv);
    }
}

#else

int main(int argc, char *argv[])
{
    return MainProgram::mainprogram(argc, argv);
}
#endif
