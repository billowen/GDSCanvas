#include <QApplication>
#include <QPushButton>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QPushButton btn("Hello world");
    btn.show();

    return app.exec();
}
