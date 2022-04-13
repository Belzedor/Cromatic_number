#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QDebug>
#include <algorithm>
#include <QRegularExpression>
#include <fstream>
#include <QMessageBox>
#include <QDir>
#include <QGraphicsScene>
#include <QPen>
#include <set>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void show_matrix();
    void show_number();
    void show_colours();
    void count_colors();
    bool is_matrix_ok(std::vector<std::vector<bool>> M);
    std::vector<std::vector<bool>> data_to_matrix(QString data);
    void save_matrix();
    QString load_data();
    void draw_graph();

private slots:
    void on_count_btn_clicked();

    void on_set_matrix_btn_clicked();

    void on_paint_graph_btn_clicked();

private:
    Ui::MainWindow *ui;

    std::vector<std::vector<bool>> matrix;
    std::vector<int> colours;
    QGraphicsScene *graph_scene;

    std::vector<std::vector<bool>> backup_matrix = {{1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
                                                    {0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
                                                    {0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
                                                    {0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
                                                    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
                                                    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
                                                    {0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
                                                    {0, 0, 1, 0, 0, 0, 0, 0, 1, 0},
                                                    {0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
                                                    {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
                                                    {0, 0, 0, 0, 0, 1, 0, 1, 0, 0},
                                                    {0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
                                                    {0, 0, 0, 0, 0, 0, 1, 0, 1, 0},
                                                    {0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
                                                    {0, 0, 0, 0, 0, 0, 0, 1, 0, 1}};

    std::vector<QPoint> coordinates = {QPoint(0, -100),
                                       QPoint(90, -40),
                                       QPoint(60, 60),
                                       QPoint(-60, 60),
                                       QPoint(-90, -40),
                                       QPoint(-40, -20),
                                       QPoint(0, -50),
                                       QPoint(40, -20),
                                       QPoint(20, 20),
                                       QPoint(-20, 20)};

    std::vector<QBrush> brushes = {QBrush(Qt::red), QBrush(Qt::green), QBrush(Qt::blue), QBrush(Qt::black), QBrush(Qt::yellow)};
};
#endif // MAINWINDOW_H
