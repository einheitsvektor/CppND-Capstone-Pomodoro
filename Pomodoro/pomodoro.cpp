#include <QDebug>
#include <filesystem>
#include <string>

#include "pomodoro.h"
#include "ui_pomodoro.h"

Pomodoro::Pomodoro(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Pomodoro)
    , confreader(Configreader())
    , timer(new QTimer)
    , time(new QTime(0, 25, 0, 0))
    , timertype(TimerType::focus)
    , isFirstRun(true)
    , runButtonClicked(false)
    , roundCount(confreader.initRoundCount())
    , focusCount(confreader.initFocusCount())
    , playlist(new QMediaPlaylist)
    , player(new QMediaPlayer)
    , playButtonClicked(false)
{
    ui->setupUi(this);

    // Connect GUI-elements to functions and methods
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    connect(ui->Run, SIGNAL(released()), this, SLOT(runPressed()));
    connect(ui->Reset, SIGNAL(released()), this, SLOT(resetPressed()));
    connect(ui->Minus, SIGNAL(released()), this, SLOT(minusPressed()));
    connect(ui->Plus, SIGNAL(released()), this, SLOT(plusPressed()));
    connect(ui->PlaySound, SIGNAL(released()), this, SLOT(playSoundPressed()));
    connect(ui->Slider, SIGNAL(valueChanged(int)), player, SLOT(setVolume(int)));

    // Set mp3-player path and infinite loops
    playlist->addMedia(QUrl("qrc:/media/White_noise.mp3"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    player->setPlaylist(playlist);
    // Volume 2/100 as of the White_noise is very loud
    player->setVolume(2);

    ui->Display->setText(time->toString("mm:ss"));
    ui->RoundDisplay->setText(QString::number(roundCount));
    ui->GoalDisplay->setText(QString::number(focusCount));
}

Pomodoro::~Pomodoro() {
    // When exiting this app, write current stats to file
    // in case of starting it again on the same day.
    confreader.setConfig(this->roundCount, this->focusCount);
    delete ui;
}

// This method will be called every second via the timer
void Pomodoro::updateTime() {
    if (time->minute() == 0 && time->second() == 0) {
        if (timertype == TimerType::focus) {
            focusCount++;
            roundCount++;
            if (focusCount % 4 == 0) {
                // Set timer to long pause
                time->setHMS(0, 15, 1);
                ui->ColorBox->setStyleSheet(BLUE); // Blue
            }
            else {
                // Set timer to short pause
                time->setHMS(0, 5, 1);
                ui->ColorBox->setStyleSheet(GREEN); // Green
            }
            timertype = TimerType::anybreak;
        }
        else {
            if (roundCount >= 4)
                roundCount = 0;
            // Get timer back to focus state and length
            time->setHMS(0, 25, 1);
            ui->ColorBox->setStyleSheet(RED); // Red
            timertype = TimerType::focus;
        }
    }
    QTime updatedTime = time->addSecs(-1);
    time->setHMS(0, updatedTime.minute(), updatedTime.second());
    ui->Display->setText(time->toString("mm:ss"));
    ui->RoundDisplay->setText(QString::number(roundCount));
    ui->GoalDisplay->setText(QString::number(focusCount));
}

// Once the run button is pressed, different colors indicate the current state
void Pomodoro::runPressed() {
    if (isFirstRun) {
        ui->ColorBox->setStyleSheet(RED); // Red
        isFirstRun = false;
        if (roundCount == 4) roundCount = 0;
    }
    if (!runButtonClicked) {
        runButtonClicked = true;
        ui->Run->setText("Pause");
        timer->start(1000);
    }
    else {
        runButtonClicked = false;
        ui->Run->setText("Start");
        timer->stop();
    }
}

// Pause timer and set it back
void Pomodoro::resetPressed() {
    timer->stop();
    time->setHMS(0, 25, 0);
    runButtonClicked = false;
    ui->Display->setText(time->toString("mm:ss"));
}

// Decrease timer by one minute
void Pomodoro::minusPressed() {
    if (time->minute() == 0 && time->second() >= 1) {
        timer->stop();
        time->setHMS(0, 0, 0);
        ui->Display->setText(time->toString("mm:ss"));
        ui->Run->setText("Start");
        runButtonClicked = false;
    }
    else if (time->minute() >= 1) {
        timer->stop();
        time->setHMS(0, time->minute()-1, 0);
        ui->Display->setText(time->toString("mm:ss"));
        ui->Run->setText("Start");
        runButtonClicked = false;
    }
}

// Increase timer by one minute
void Pomodoro::plusPressed() {
    if (time->minute() <= 58) {
        timer->stop();
        time->setHMS(0, time->minute()+1, 0);
        ui->Display->setText(time->toString("mm:ss"));
        ui->Run->setText("Start");
        runButtonClicked = false;
    }
}

// Play in constructor loaded sound as infinite loop
void Pomodoro::playSoundPressed() {
    if (!playButtonClicked) {
        player->play();
        playButtonClicked = true;
        ui->PlaySound->setText("Stop");
    }
    else if (playButtonClicked) {
        player->stop();
        playButtonClicked = false;
        ui->PlaySound->setText("Play");
    }
}
