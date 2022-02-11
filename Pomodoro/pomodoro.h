#ifndef POMODORO_H
#define POMODORO_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
// #include <QMediaPlayer>
#include <QMediaPlaylist>

#include "configreader.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Pomodoro; }
QT_END_NAMESPACE

enum class TimerType {
    focus,
    anybreak
};

class Pomodoro : public QMainWindow {
    Q_OBJECT

public:
    Pomodoro(QWidget *parent = nullptr);
    ~Pomodoro();

// Qt slots to connect with different signals to interact with the GUI
private slots:
    void updateTime();
    void runPressed();
    void resetPressed();
    void minusPressed();
    void plusPressed();
    // void playSoundPressed();

private:
    Ui::Pomodoro *ui;

    Configreader confreader;

    QTimer *timer;
    QTime *time;
    TimerType timertype;
    bool isFirstRun;
    bool runButtonClicked;
    int roundCount;
    int focusCount;
    // QMediaPlaylist *playlist;
    // QMediaPlayer *player;
    // bool playButtonClicked;

    QString RED = "background-color: rgb(255, 0, 0);";
    QString GREEN = "background-color: rgb(50, 150, 50);";
    QString BLUE = "background-color: rgb(0, 0, 30);";

    int rounds = 3;
};
#endif // POMODORO_H
