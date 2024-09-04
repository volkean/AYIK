#include "ayikoption.h"
#include <QFile>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QProgressDialog>
#include <QtWidgets/QLabel>

AyikOption* AyikOption::instance;

AyikOption::AyikOption()
{
    init();
}
void AyikOption::init()
{
}
void AyikOption::postInit()
{
    tableHeaders<<tr("Tag")<<tr("Name")<<tr("Rating")<<tr("Meaning");
    ratingLevels<<"1"<<"2"<<"3"<<"4"<<"5";
    freqLevels<<"1"<<"5"<<"10"<<"20";
    tagAllWords=tr("All");
    txt_meaning_changed=false;

    tabs = new QTabWidget();
    QWidget* central = new QWidget();
    QWidget* wordswidget = new QWidget();
    QWidget* helpwidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(central);

    //-- frequency
    group_freq = new QGroupBox(tr("Frequency"));
    cmb_freq = new QComboBox();
    cmb_freq->setEditable(true);
    cmb_freq->addItems(freqLevels);
    QLabel *lbl_freq = new QLabel(tr("Frequency(mins)"));
    QHBoxLayout *freqlayout = new QHBoxLayout(group_freq);
    freqlayout->addWidget(lbl_freq);
    freqlayout->addWidget(cmb_freq);
    freqlayout->addStretch();

    //-- tag
    group_tag = new QGroupBox(tr("Tag"));
    //cmb_tag = new QComboBox();
    //cmb_freq->setEditable(true);
    tagslist = new QListWidget();
    QLabel *lbl_tag = new QLabel(tr("Tag"));
    btn_tag_select = new QPushButton(tr("Choose"));
    lineedit_tag = new QLineEdit();
    lineedit_tag->setText(tagAllWords);
    lineedit_tag->setReadOnly(true);
    QVBoxLayout *tag2layout = new QVBoxLayout(group_tag);
    QHBoxLayout *taglayout = new QHBoxLayout();
    taglayout->addWidget(lbl_tag);
    //taglayout->addWidget(cmb_tag);
    taglayout->addWidget(lineedit_tag);
    taglayout->addWidget(btn_tag_select);
    taglayout->addStretch();
    //todo
    tagslist->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed));
    tagslist->setFixedHeight(60);
    tag2layout->addLayout(taglayout);
    tag2layout->addWidget(tagslist);
    //tag2layout->addStretch();

    //-- source history
    group_src_hist = new QGroupBox(tr("Word Level"));

    //-- history
    rating = new AyikRating(false,true,1,5);
    QLabel *lbl_rating = new QLabel(tr("Level"));
    cmb_rating_oper = new QComboBox();
    QStringList items_rating;
    items_rating << ">=" << "=" << "<=" ;
    cmb_rating_oper->addItems(items_rating);

    QHBoxLayout *groupsrchistlayout = new QHBoxLayout(group_src_hist);
    groupsrchistlayout->addWidget(lbl_rating);
    groupsrchistlayout->addWidget(cmb_rating_oper);
    groupsrchistlayout->addWidget(rating);
    groupsrchistlayout->addStretch();

    layout->addWidget(group_freq);
    layout->addWidget(group_tag);
    layout->addWidget(group_src_hist);
    layout->addStretch();

    //-- words    

    QVBoxLayout *wwlayout = new QVBoxLayout(wordswidget);

    btn_add_file = new QPushButton(tr("Add..."));
    btn_delete_word = new QPushButton(tr("Delete"));
    btn_delete_all = new QPushButton(tr("Delete All"));
    btn_update_meaning = new QPushButton(QIcon(":/images/ok.png"),tr("Update"));
    lineedit_word_filter = new QLineEdit();

    db = AyikDB::getInstance()->db;

    tableview = new TableView(db);
    tableview->model->setTable("words");
    tableview->model->select();

    tableview->model->setHeaderData(0, Qt::Horizontal, QObject::tr("Id"));
    tableview->model->setHeaderData(1, Qt::Horizontal, QObject::tr("Tag"));
    tableview->model->setHeaderData(2, Qt::Horizontal, QObject::tr("Name"));
    tableview->model->setHeaderData(3, Qt::Horizontal, QObject::tr("Meaning"));
    tableview->model->setHeaderData(4, Qt::Horizontal, QObject::tr("Rating"));
    //tableview->model->setHeaders(tableHeaders);
    tableview->setColumnHidden(0,true);
    tableview->setColumnHidden(3,true);
    tableview->setColumnHidden(4,true);
    tableview->setModel(tableview->model);

    txt_meaning = new QTextEdit();
    QHBoxLayout *wwbtnlayout = new QHBoxLayout();
    wwbtnlayout->addWidget(btn_add_file);
    wwbtnlayout->addWidget(btn_delete_word);
    wwbtnlayout->addWidget(btn_delete_all);
    wwbtnlayout->addStretch();

    QHBoxLayout *wwfilterlayout = new QHBoxLayout();
    wwfilterlayout->addWidget(new QLabel(tr("Filter:")));
    wwfilterlayout->addWidget(lineedit_word_filter);
    wwbtnlayout->addStretch();

    wwlayout->addLayout(wwbtnlayout);
    wwlayout->addLayout(wwfilterlayout);
    QSplitter *word_meaning_splitter = new QSplitter(Qt::Vertical,this);
    word_meaning_splitter->addWidget(tableview);
    word_meaning_splitter->addWidget(txt_meaning);
    word_meaning_splitter->addWidget(btn_update_meaning);

    wwlayout->addWidget(word_meaning_splitter);

    //help
    QVBoxLayout *helplayout = new QVBoxLayout();
    QTextEdit *txt_help = new QTextEdit();
    txt_help->setReadOnly(true);
    QFile file(":/images/help.html");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString text = in.readAll();
        txt_help->setHtml(text);
    }

    helplayout->addWidget(txt_help);
    helpwidget->setLayout(helplayout);

    tabs->addTab(central,tr("General"));
    tabs->addTab(wordswidget,tr("Words"));
    tabs->addTab(helpwidget,tr("Help"));
    setCentralWidget(tabs);

    tagslist->setVisible(false);//after initing
    createActions();

}
AyikOption* AyikOption::getInstance()
{
    if(instance==NULL) instance = new AyikOption();
    return instance;
}
int AyikOption::getMessageFrequency() {
    QString siklik = cmb_freq->currentText();
    return siklik.toInt();
}
QString AyikOption::getTag()
{
    QString tag="";
    /*if(cmb_tag->currentIndex()>0) {
        tag = cmb_tag->currentText();
    }*/
    tag = lineedit_tag->text();
    if(tag == tagAllWords) tag="";
    return tag;
}
QString AyikOption::getRating()
{
    return rating->getRating();
}
QString AyikOption::getRatingOperator()
{
    QString ratingoper = cmb_rating_oper->currentText();
    return ratingoper;
}
void AyikOption::createActions()
{
    connect(btn_add_file,SIGNAL(clicked()),this,SLOT(slot_add_file()));
    connect(btn_delete_word,SIGNAL(clicked()),this,SLOT(slot_delete_word()));
    connect(btn_delete_all,SIGNAL(clicked()),this,SLOT(slot_delete_all()));
    connect(btn_update_meaning,SIGNAL(clicked()),this,SLOT(slot_update_meaning()));
    //connect(cmb_tag,SIGNAL(clicked()),this,SLOT(slot_add_file()));//todo
    connect(tableview->selectionModel(),SIGNAL(currentRowChanged(const QModelIndex&,const QModelIndex&)),this,SLOT(slot_word_selected(const QModelIndex&,const QModelIndex&)));
    connect(txt_meaning,SIGNAL(textChanged()),this,SLOT(slot_txt_meaning_changed()));
    connect(lineedit_word_filter,SIGNAL(textChanged(const QString&)),this,SLOT(slot_filterTextChanged(const QString&)));
    connect(btn_tag_select,SIGNAL(clicked()),this,SLOT(slot_chooseTag()));
    connect(tagslist,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slot_tagChosen(QListWidgetItem*)));

}
void AyikOption::closeEvent(QCloseEvent *event)
{
    //todo: save & load
    event->ignore();
    //    loadCheckedFileList();
    hide();
}

void AyikOption::slot_add_file()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Words File"), "", tr("Text Files (*.txt)"));
    if(fileName!="") {
        bool tagOk;
        QString tag = QInputDialog::getText(this, tr("Tag"),tr("Tag:"), QLineEdit::Normal, "", &tagOk);
        if (tagOk && !tag.isEmpty()) {
            bool ratingOk;
            QString rating = QInputDialog::getItem(this, tr("Rating"), tr("Rating:"), ratingLevels, 2, false, &ratingOk);
            if (ratingOk && !rating.isEmpty()) {
                //qDebug()<<fileName<<tag<<rating;
                //QMessageBox::information(0,"info",fileName+" "+tag+" "+rating);
                AyikDB::getInstance()->addWords(fileName,tag,rating);
                tableview->model->select();//reload list
            }
        }
    }
}

void AyikOption::slot_delete_word()
{
    QString err;
    QModelIndexList selection = tableview->selectionModel()->selectedRows(1);
    QProgressDialog pd(tr("Deleting.."), tr("Cancel"), 0, selection.size());
    pd.setWindowModality(Qt::WindowModal);
    int steps=0;
    QList<AyikWord> wordlist;
    for(int i=selection.size() - 1;i >=0 ;i--) {
        if (pd.wasCanceled()) break;
        QModelIndex index = selection.at(i);

        QString name = tableview->model->data(index,Qt::EditRole).toString();
        AyikWord w;
        w.setName(name);
        wordlist.append(w);
        if(wordlist.size()>50) {
            AyikDB::getInstance()->deleteWords(wordlist,err);
            steps+=wordlist.size();
            wordlist.clear();
            pd.setValue(steps);
        }
        tableview->model->removeRow(index.row());
    }
    if(wordlist.size()>0) {
        AyikDB::getInstance()->deleteWords(wordlist,err);
        steps+=wordlist.size();
        wordlist.clear();
        pd.setValue(steps);
    }

    currword.setName("");
    txt_meaning->clear();

    tableview->model->select();
}

void AyikOption::slot_delete_all()
{
    QString err;
    AyikDB::getInstance()->deleteWordsLike(lineedit_word_filter->text(), err);

    currword.setName("");
    txt_meaning->clear();

    tableview->model->select();
}

void AyikOption::slot_update_meaning()
{
    currword.setMeaning(txt_meaning->toPlainText().replace("\n","<br>"));
    AyikDB::getInstance()->updateWord(currword);
    tableview->model->select();
}

void AyikOption::slot_word_selected(const QModelIndex &current,const QModelIndex &previous)
{
    if(current.isValid()) {
        QModelIndex nameIndex = tableview->model->index(current.row(), 2);
        QModelIndex meaningIndex = tableview->model->index(current.row(), 3);
        QString currwordname = tableview->model->data(nameIndex).toString();
        QString currwordmeaning = tableview->model->data(meaningIndex).toString();
        QString currerr;
        AyikDB::getInstance()->getWord(currword,currwordname,currerr);
        currwordmeaning = currword.getMeaning();
        txt_meaning->setHtml(currwordmeaning);
    } else {
        currword.setName("");
        txt_meaning->clear();
    }
}
void AyikOption::slot_txt_meaning_changed()
{
    txt_meaning_changed=true;
}
void AyikOption::slot_filterTextChanged(const QString& text)
{
    tableview->model->setFilter("tag like '%"+text+"%' or name like '%"+text+"%'");
    tableview->model->select();
}

void AyikOption::showHelp()
{
    tabs->setCurrentIndex(2);
    setVisible(true);
}
void AyikOption::slot_chooseTag()
{
    reloadTagList();
    tagslist->setVisible(true);
}
void AyikOption::reloadTagList()
{
    QStringList items_tag;
    QString err;
    AyikDB::getInstance()->getTagList(items_tag,err);//todo: err check
    items_tag.push_front(tagAllWords);
    tagslist->clear();
    tagslist->addItems(items_tag);
}
void AyikOption::slot_tagChosen(QListWidgetItem * item)
{
    lineedit_tag->setText(item->text());
    tagslist->setVisible(false);
}
