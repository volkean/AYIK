#include "ayikpopup.h"
#include "ayikdb.h"
#include "ayikoption.h"
#include "ayikweb.h"
#include <QtWidgets/QApplication>

AyikPopup* AyikPopup::instance;

AyikPopup::AyikPopup()
{
    instance = this;

    setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint);
    setGeometry(0,0,320,160);
    setObjectName("ayikpopup");
    //setStyleSheet("#ayikpopup {background-color: rgb(0, 170, 255);} ");
    setStyleSheet("#ayikpopup {   	background-color: qlineargradient(spread:reflect, x1:0.130682, y1:0.392, x2:1, y2:0.977955, stop:0 rgba(96, 221, 255, 255), stop:1 rgba(255, 255, 255, 255));   }      .QWidget {    background-color: beige;   }      /* Nice Windows-XP-style password character. */   QLineEdit {   lineedit-password-character: 9679;   }      /* We provide a min-width and min-height for push buttons   so that they look elegant regardless of the width of the text. */   QPushButton {   background-color: palegoldenrod;   border-width: 2px;   border-color: darkkhaki;   border-style: solid;   border-radius: 5;   padding: 3px;   }      QPushButton:hover {   background-color: khaki;   }      /* Increase the padding, so the text is shifted when the button is   pressed. */   QPushButton:pressed {   padding-left: 5px;   padding-top: 5px;   background-color: #d0d67c;   }      QLabel, QAbstractButton {   font: bold;   }      /* Mark mandatory fields with a brownish color. */   .mandatory {   color: brown;   }      /* Bold text on status bar looks awful. */   QStatusBar QLabel {   font: normal;   }      QStatusBar::item {   border-width: 1;   border-color: darkkhaki;   border-style: solid;   border-radius: 2;   }      QComboBox, QLineEdit, QSpinBox, QTextEdit, QListView {   background-color: cornsilk;   selection-color: #0a214c;    selection-background-color: #C19A6B;   }      QListView {   show-decoration-selected: 1;   }      QListView::item:hover {   background-color: wheat;   }      /* We reserve 1 pixel space in padding. When we get the focus,   we kill the padding and enlarge the border. This makes the items   glow. */   QLineEdit {   border-width: 2px;   padding: 1px;   border-style: solid;   border-color: darkkhaki;   border-radius: 5px;   }      /* As mentioned above, eliminate the padding and increase the border. */   QLineEdit:focus, QFrame:focus {   border-width: 3px;   padding: 0px;   }      /* A QLabel is a QFrame ... */   QLabel {   border: none;   padding: 0;   background: none;   }      /* A QToolTip is a QLabel ...    QToolTip {   border: 2px solid darkkhaki;   padding: 5px;   border-radius: 3px;   opacity: 200;   }*/      /* Nice to have the background color change when hovered. */   QRadioButton:hover, QCheckBox:hover {   background-color: wheat;   }      /* Force the dialog's buttons to follow the Windows guidelines. */   QDialogButtonBox {   button-layout: 0;   }         ");

    layout = new QVBoxLayout(this);

    QHBoxLayout *upperlayout = new QHBoxLayout();
    QHBoxLayout *middlelayout = new QHBoxLayout();
    rating = new StarRating(5,5);
    rating->setEditable(true);
    btnClose = new QPushButton(QIcon(":/images/close.png"),"");
    btnPrev = new QPushButton(QIcon(":/images/back-32.png"),"");
    btnNext = new QPushButton(QIcon(":/images/next-32.png"),"");
    btnReveal = new QPushButton(QIcon(":/images/helpCursor.png"),"");
    btnReveal->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    btnClose->setToolTip(tr("Close"));
    btnNext->setToolTip(tr("Next"));
    btnReveal->setToolTip(tr("Reveal"));

    labelTag = new QLabel();
    labelWord = new QLabel();
    QFont font = labelWord->font();
    font.setBold(true);
    labelWord->setFont(font);

    txtMeaning = new QTextEdit();
    txtMeaning->setTextColor(QColor::fromRgb(0,0,0));
    txtMeaning->setReadOnly(true);
    labelLoading = new QLabel("Loading");
    //bar_loading = new QProgressBar();
    loading = new LoadingWidget();
    loading->setVisible(false);
    /*bar_loading->setFormat("Loading");
    bar_loading->setTextVisible(true);
    bar_loading->setMaximum(0);
    bar_loading->setMinimum(0);*/

    //QFrame *frame_help = new QFrame();
    //frame_help->setFrameStyle(QFrame::Box | QFrame::Sunken);
    //frame_help->setFrameStyle(QFrame::NoFrame);
    //frame_help->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QVBoxLayout *helplayout = new QVBoxLayout();
    helplayout->addWidget(loading);
    helplayout->addWidget(txtMeaning);
    //helplayout->addWidget(btn_reveal);

    upperlayout->addWidget(rating);
    upperlayout->addStretch();
    upperlayout->addWidget(labelTag);
    upperlayout->addWidget(btnClose);

    middlelayout->addWidget(btnPrev);
    middlelayout->addWidget(btnNext);
    middlelayout->addWidget(btnReveal);
    middlelayout->addWidget(labelWord);
    middlelayout->addStretch();
    btnPrev->setVisible(false);

    layout->addLayout(upperlayout);
    /*QLabel *label = new QLabel();
    label->setFrameStyle(QFrame::HLine);*/

    //layout->addWidget(hline);
    layout->addLayout(middlelayout);
    layout->addLayout(helplayout);
    layout->addStretch();

    createActions();
    AyikDB::getInstance();//lets instantiate db
}
AyikPopup* AyikPopup::getInstance()
{
    if(instance==NULL) instance = new AyikPopup();
    return instance;
}

void AyikPopup::showPopup()
{
    nextWord();
    //screen geometry
    QRect scr = QGuiApplication::primaryScreen()->geometry();
    setGeometry(scr.width() - width() - 10,scr.height() - height() - 40,width(),height());
    show();
}
void AyikPopup::hidePopup()
{
    hide();
    emit windowHidden();
}
void AyikPopup::nextWord()
{
    QString err;
    AyikOption* ayikOption = AyikOption::getInstance();
    AyikDB* db = AyikDB::getInstance();
    db->getRandomWord(currentAyikWord,ayikOption->getTag(),ayikOption->getRating(),ayikOption->getRatingOperator(),err);
    if(err!="") qDebug()<<err;
    prepareUiWithCurrentWord();
}
void AyikPopup::prevWord()
{

}
void AyikPopup::revealWord()
{
    emit beforeReveal();
    if(currentAyikWord.getMeaning().trimmed().isEmpty()) {
        ayikWeb.lookupWordMeaning(currentAyikWord);
        //afterReveal() will be emitted when lookup done
    } else {
        emit afterReveal();
    }
}
void AyikPopup::createActions()
{
    connect(btnClose,SIGNAL(clicked()),this,SLOT(hidePopup()));
    connect(btnNext,SIGNAL(clicked()),this,SLOT(nextWord()));
    connect(btnPrev,SIGNAL(clicked()),this,SLOT(prevWord()));
    connect(btnReveal,SIGNAL(clicked()),this,SLOT(revealWord()));
    connect(rating,SIGNAL(starCountChanged(int)),this,SLOT(updateRating(int)));
    connect(this,SIGNAL(beforeReveal()),this,SLOT(slotBeforeReveal()));
    connect(this,SIGNAL(afterReveal()),this,SLOT(slotAfterReveal()));
    connect(&ayikWeb,SIGNAL(lookupDone()),this,SLOT(slotLookupDone()));
}

void AyikPopup::prepareUiWithCurrentWord()
{
    //hide reveal frame
    loading->setVisible(false);
    txtMeaning->setVisible(false);
    //set values
    labelWord->setText(currentAyikWord.getName());
    rating->setStarCount(currentAyikWord.getRating().toInt());
    labelTag->setText(currentAyikWord.getTag());
}

void AyikPopup::updateRating(int rating)
{
    currentAyikWord.setRating(QString::number(rating));
    AyikDB* db = AyikDB::getInstance();
    db->updateWord(currentAyikWord);
}
void AyikPopup::slotBeforeReveal()
{
    loading->setVisible(true);
    txtMeaning->setVisible(false);
    //retrieve meaning if not present at hand
    if(currentAyikWord.getMeaning().isEmpty()) {
        ayikWeb.lookupWordMeaning(currentAyikWord);
    }
}
void AyikPopup::slotAfterReveal()
{
    AyikDB* db = AyikDB::getInstance();
    db->updateWord(currentAyikWord);
    //txt_meaning->setPlainText(currentword.getMeaning());
    txtMeaning->setHtml(currentAyikWord.getMeaning());
    loading->setVisible(false);
    txtMeaning->setVisible(true);
}
void AyikPopup::slotLookupDone()
{
    currentAyikWord.setMeaning(ayikWeb.getLastWordMeaning());
    emit afterReveal();
}
