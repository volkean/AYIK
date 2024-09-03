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

class AyikPopup : public QFrame /*QWidget*/
{
    Q_OBJECT

public:
    AyikPopup();
    static AyikPopup* getInstance();
    void showPopup();

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
    void prepareUiWithCurrentWord();
    void createActions();

    static AyikPopup* instance;
    QGridLayout *grid;
    QVBoxLayout *layout;
    StarRating *rating;
    QPushButton *btnClose;
    QPushButton *btnPrev;
    QPushButton *btnNext;
    QPushButton *btnReveal;
    QLabel *labelTag;
    QLabel *labelWord;
    QTextEdit *txtMeaning;
    QLabel *labelLoading;
    QProgressBar *barLoading;
    LoadingWidget *loading;

    AyikWord currentAyikWord;
    AyikWeb ayikWeb;

signals:
    void windowHidden();
    void beforeReveal();
    void afterReveal();
};

#endif // AYIKPOPUP_H
