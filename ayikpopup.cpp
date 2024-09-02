#include "ayikpopup.h"
#include "ayikdb.h"
#include "ayikoption.h"
#include "ayikweb.h"
#include <QtWidgets/QApplication>

ayikPopup* ayikPopup::instance;

ayikPopup::ayikPopup()
{
    instance = this;
    aweb.start();

    setWindowFlags(Qt::ToolTip | Qt::WindowStaysOnTopHint);
    //setWindowFlags(Qt::WindowStaysOnTopHint);
    setGeometry(0,0,320,160);
    setObjectName("ayikpopup");
    //setStyleSheet("#ayikpopup {background-color: rgb(0, 170, 255);} ");
    setStyleSheet("#ayikpopup {   	background-color: qlineargradient(spread:reflect, x1:0.130682, y1:0.392, x2:1, y2:0.977955, stop:0 rgba(96, 221, 255, 255), stop:1 rgba(255, 255, 255, 255));   }      .QWidget {    background-color: beige;   }      /* Nice Windows-XP-style password character. */   QLineEdit {   lineedit-password-character: 9679;   }      /* We provide a min-width and min-height for push buttons   so that they look elegant regardless of the width of the text. */   QPushButton {   background-color: palegoldenrod;   border-width: 2px;   border-color: darkkhaki;   border-style: solid;   border-radius: 5;   padding: 3px;   }      QPushButton:hover {   background-color: khaki;   }      /* Increase the padding, so the text is shifted when the button is   pressed. */   QPushButton:pressed {   padding-left: 5px;   padding-top: 5px;   background-color: #d0d67c;   }      QLabel, QAbstractButton {   font: bold;   }      /* Mark mandatory fields with a brownish color. */   .mandatory {   color: brown;   }      /* Bold text on status bar looks awful. */   QStatusBar QLabel {   font: normal;   }      QStatusBar::item {   border-width: 1;   border-color: darkkhaki;   border-style: solid;   border-radius: 2;   }      QComboBox, QLineEdit, QSpinBox, QTextEdit, QListView {   background-color: cornsilk;   selection-color: #0a214c;    selection-background-color: #C19A6B;   }      QListView {   show-decoration-selected: 1;   }      QListView::item:hover {   background-color: wheat;   }      /* We reserve 1 pixel space in padding. When we get the focus,   we kill the padding and enlarge the border. This makes the items   glow. */   QLineEdit {   border-width: 2px;   padding: 1px;   border-style: solid;   border-color: darkkhaki;   border-radius: 5px;   }      /* As mentioned above, eliminate the padding and increase the border. */   QLineEdit:focus, QFrame:focus {   border-width: 3px;   padding: 0px;   }      /* A QLabel is a QFrame ... */   QLabel {   border: none;   padding: 0;   background: none;   }      /* A QToolTip is a QLabel ...    QToolTip {   border: 2px solid darkkhaki;   padding: 5px;   border-radius: 3px;   opacity: 200;   }*/      /* Nice to have the background color change when hovered. */   QRadioButton:hover, QCheckBox:hover {   background-color: wheat;   }      /* Force the dialog's buttons to follow the Windows guidelines. */   QDialogButtonBox {   button-layout: 0;   }         ");

    layout = new QVBoxLayout(this);

    QHBoxLayout *upperlayout = new QHBoxLayout();
    QHBoxLayout *middlelayout = new QHBoxLayout();
    rating = new starrating(5,5);
    rating->setEditable(true);
    btn_close = new QPushButton(QIcon(":/images/close.png"),"");
    btn_prev = new QPushButton(QIcon(":/images/back-32.png"),"");
    btn_next = new QPushButton(QIcon(":/images/next-32.png"),"");
    btn_reveal = new QPushButton(QIcon(":/images/helpCursor.png"),"");
    btn_reveal->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    btn_close->setToolTip(tr("Close"));
    btn_next->setToolTip(tr("Next"));
    btn_reveal->setToolTip(tr("Reveal"));

    label_tag = new QLabel();
    label_word = new QLabel();
    QFont font = label_word->font();
    font.setBold(true);
    label_word->setFont(font);

    txt_meaning = new QTextEdit();
    txt_meaning->setTextColor(QColor::fromRgb(0,0,0));
    txt_meaning->setReadOnly(true);
    label_loading = new QLabel("Loading");
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
    helplayout->addWidget(txt_meaning);
    //helplayout->addWidget(btn_reveal);

    upperlayout->addWidget(rating);
    upperlayout->addStretch();
    upperlayout->addWidget(label_tag);
    upperlayout->addWidget(btn_close);

    middlelayout->addWidget(btn_prev);
    middlelayout->addWidget(btn_next);
    middlelayout->addWidget(btn_reveal);
    middlelayout->addWidget(label_word);
    middlelayout->addStretch();
    btn_prev->setVisible(false);

    layout->addLayout(upperlayout);
    /*QLabel *label = new QLabel();
    label->setFrameStyle(QFrame::HLine);*/

    //layout->addWidget(hline);
    layout->addLayout(middlelayout);
    layout->addLayout(helplayout);
    layout->addStretch();

    createActions();
    ayikDB::getInstance();//lets instantiate db
}
ayikPopup* ayikPopup::getInstance()
{
    if(instance==NULL) instance = new ayikPopup();
    return instance;
}

void ayikPopup::showPopup()
{
    //next word
    nextWord();
    //screen geometry
    //QRect scr = QApplication::desktop()->screenGeometry();
    QRect scr = QGuiApplication::primaryScreen()->geometry();

    setGeometry(scr.width() - width() - 10,scr.height() - height() - 40,width(),height());
    show();
}
void ayikPopup::hidePopup()
{
    hide();
    emit window_hidden();
}
void ayikPopup::nextWord()
{
    QString err;
    ayikOption* ao = ayikOption::getInstance();
    ayikDB* db = ayikDB::getInstance();
    //db->getRandomWord(w);
    db->getRandomWordMemdb(currentword,ao->getTag(),ao->getRating(),ao->getRatingOperator(),err);
    if(err!="") qDebug()<<err;
    setWord(currentword);
}
void ayikPopup::prevWord()
{

}
void ayikPopup::revealWord()
{
    emit beforeReveal();
    if(currentword.getMeaning().trimmed().isEmpty()) {
        QString word = currentword.getName();
        aweb.lookupWord(word);
    } else {
        emit afterReveal();
    }
}
void ayikPopup::createActions()
{
    connect(btn_close,SIGNAL(clicked()),this,SLOT(hidePopup()));
    connect(btn_next,SIGNAL(clicked()),this,SLOT(nextWord()));
    connect(btn_prev,SIGNAL(clicked()),this,SLOT(prevWord()));
    connect(btn_reveal,SIGNAL(clicked()),this,SLOT(revealWord()));
    connect(rating,SIGNAL(starCountChanged(int)),this,SLOT(updateRating(int)));
    connect(this,SIGNAL(beforeReveal()),this,SLOT(slotBeforeReveal()));
    connect(this,SIGNAL(afterReveal()),this,SLOT(slotAfterReveal()));
    connect(&aweb,SIGNAL(lookupDone()),this,SLOT(slotLookupDone()));
}

void ayikPopup::setWord(const ayikWord& w)
{
    //hide reveal frame
    loading->setVisible(false);
    txt_meaning->setVisible(false);
    //set values
    label_word->setText(w.getName());
    rating->setStarCount(w.getRating().toInt());
    label_tag->setText(w.getTag());
}

void ayikPopup::updateRating(int rating)
{
    currentword.setRating(QString::number(rating));
    ayikDB* db = ayikDB::getInstance();
    db->updateWord(currentword);
}
void ayikPopup::slotBeforeReveal()
{
    loading->setVisible(true);
    txt_meaning->setVisible(false);
}
void ayikPopup::slotAfterReveal()
{
    txt_meaning->setPlainText(currentword.getMeaning());
    //txt_meaning->setHtml(currentword.getMeaning());
    loading->setVisible(false);
    txt_meaning->setVisible(true);
}
void ayikPopup::slotLookupDone()
{
    currentword.setMeaning(aweb.getAnswer());
    slotAfterReveal();
}
