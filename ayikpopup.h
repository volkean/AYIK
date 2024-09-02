#ifndef AYIKPOPUP_H
#define AYIKPOPUP_H

#include <QtGui>
#include "starrating.h"
#include "ayikword.h"
#include "dictutil.h"
#include "ayikweb.h"
#include "loadingwidget.h"
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStyleOptionViewItem>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>

class ayikPopup : public QFrame /*QWidget*/
{
    Q_OBJECT

public:
    ayikPopup();
    static ayikPopup* getInstance();
    void showPopup();
    void setWord(const ayikWord& w);

protected slots:
    void hidePopup();
    void nextWord();
    void prevWord();
    void revealWord();
    void updateRating(int rating);
    void slotBeforeReveal();
    void slotAfterReveal();
    void slotLookupDone();

private:
    void createActions();

    static ayikPopup* instance;
    QGridLayout *grid;
    QVBoxLayout *layout;
    starrating *rating;
    QPushButton *btn_close;
    QPushButton *btn_prev;
    QPushButton *btn_next;
    QPushButton *btn_reveal;
    QLabel *label_tag;
    QLabel *label_word;
    QTextEdit *txt_meaning;
    QLabel *label_loading;
    QProgressBar *bar_loading;
    LoadingWidget *loading;

    ayikWord currentword;
    ayikWeb aweb;

signals:
    void window_hidden();
    void beforeReveal();
    void afterReveal();
};

#endif // AYIKPOPUP_H
