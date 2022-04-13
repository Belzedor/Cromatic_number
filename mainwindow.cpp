#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    std::vector<std::vector<bool>> new_matrix;
    new_matrix = data_to_matrix(load_data());

    if(is_matrix_ok(new_matrix))
    {
        matrix = data_to_matrix(load_data());
        show_matrix();
    }
    else
    {
        matrix = backup_matrix;
        show_matrix();
        QMessageBox::warning(this, "Ошибка чтения", "Загруженная матрица неверна. Загружена матрица по-умолчанию", QMessageBox::Ok);
        save_matrix();
    }

    graph_scene = new QGraphicsScene;
    ui->graph_view->setScene(graph_scene);
    ui->graph_view->scale(2, 2);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete graph_scene;
}

void MainWindow::show_matrix()
{
    ui->matrix_output->clear();
    for (auto row : matrix)
    {
        for (auto num : row)
        {
            ui->matrix_output->insertPlainText(QString::number((int)num) + " ");
        }
        ui->matrix_output->appendPlainText("");
    }
}

void MainWindow::show_number()
{
    ui->chromatic_label->setText("Хроматическое число: " + QString::number(*std::max_element(colours.begin(), colours.end())));
}

void MainWindow::show_colours()
{
    ui->colours_output->clear();
    ui->colours_output->insertPlainText("Вершины:  ");
    for (size_t i = 1; i <= matrix[0].size() ; i++)
    {
        ui->colours_output->insertPlainText(QString::number(i) + " ");
    }
    ui->colours_output->appendPlainText("");
    ui->colours_output->insertPlainText("Цвета:        ");
    for (auto colour : colours)
    {
        ui->colours_output->insertPlainText(QString::number(colour) + " ");
    }
}

void MainWindow::count_colors()
{
    colours.clear();
    colours.insert(colours.begin(), matrix[0].size(), 0);
    for (size_t vertex = 0; vertex < matrix[0].size(); vertex++)
    {
        colours[vertex]++;
        for(size_t edge = 0; edge < matrix.size(); edge++)
        {
            if (matrix[edge][vertex])
            {
                for (size_t i = 0; i < matrix[0].size(); i++)
                {
                    if(i == vertex) continue;
                    if(matrix[edge][i] && colours[vertex] == colours[i])
                    {
                        colours[vertex]++;
                        edge = 0;
                        continue;
                    }
                }
            }
        }
    }
}

bool MainWindow::is_matrix_ok(std::vector<std::vector<bool>> M)
{
    size_t first_size = M[0].size();
    if(first_size < 2) return false;
    for (auto row : M)
    {
        if (row.size() != first_size) return false;
    }
    return true;
}

std::vector<std::vector<bool>> MainWindow::data_to_matrix(QString data)
{
    std::vector<std::vector<bool>> new_matrix;
    std::vector<bool> temp;
    QStringList strList;
    strList = data.split(QRegularExpression("[\n]"), Qt::SkipEmptyParts);
    if(strList.isEmpty())
    {
        return new_matrix = {{0}};
    }
    for(auto row : strList)
    {
        for(auto simbol : row)
        {
            if(simbol.isDigit())
            {
                temp.push_back(simbol.digitValue());
            }
        }
        new_matrix.push_back(temp);
        temp.clear();
    }
    return new_matrix;
}

void MainWindow::save_matrix()
{
    std::ofstream fout;
    if(!QDir("Files").exists())
    {
        QDir().mkdir("Files");
    }
    fout.open("Files/Matrix.txt");
    if(fout.is_open())
    {
        fout << ui->matrix_output->toPlainText().toStdString();
    }
    else
    {
        QMessageBox::warning(this, "Ошибка записи", "Не удалось открыть файл.", QMessageBox::Ok);
    }
    fout.close();
}

QString MainWindow::load_data()
{
    std::string data;
    char ch;
    std::ifstream fin;
    fin.open("Files/Matrix.txt");
    if(fin.is_open())
    {
        while(fin.get(ch))
        {
            data.push_back(ch);
        }
        fin.close();
        return QString::fromStdString(data);
    }
    else
    {
        QMessageBox::warning(this, "Ошибка чтения", "Не удалось открыть файл", QMessageBox::Ok);
        fin.close();
        return QString::fromStdString("");
    }
}

void MainWindow::draw_graph()
{
    graph_scene->clear();
    int dot_size = 10;
    int from;
    QPen edge_pen(Qt::black);
    std::vector<QPoint> coordinates;
    for(size_t i = 0 ; i < matrix[0].size(); i++) coordinates.push_back(QPoint(rand()%100,rand()%100));

    for (auto row : matrix)
    {
        from = -1;
        for (size_t to = 0; to < row.size(); to++)
        {
            if (from == -1 && row[to])
            {
                from = to;
            }
            else
            {
                if(row[to]) graph_scene->addLine(coordinates[from].x() + dot_size/2, coordinates[from].y() + dot_size/2, coordinates[to].x() + dot_size/2, coordinates[to].y() + dot_size/2);
                continue;
            }
        }
    }

    for(size_t i = 0 ; i < matrix[0].size(); i++) graph_scene->addEllipse(coordinates[i].x(), coordinates[i].y(), dot_size, dot_size, edge_pen, brushes[colours[i]-1]);
}

void MainWindow::on_count_btn_clicked()
{
   count_colors();
   show_number();
   show_colours();
}


void MainWindow::on_set_matrix_btn_clicked()
{
    std::vector<std::vector<bool>> new_matrix;
    new_matrix = data_to_matrix(ui->matrix_output->toPlainText());

    if (is_matrix_ok(new_matrix))
    {
        matrix = new_matrix;
        show_matrix();
        save_matrix();
    }
    else
    {
        show_matrix();
        QMessageBox::warning(this, "Ошибка", "Введенная матрица неверна. Изменения не сохранены.", QMessageBox::Ok);
        return;
    }
}

void MainWindow::on_paint_graph_btn_clicked()
{
    if(!colours.empty()) draw_graph();
}

