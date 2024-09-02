#include "ayikoption.h"
#include "databaseconnection.h"
#include <QFile>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QProgressDialog>

ayikOption* ayikOption::instance;

ayikOption::ayikOption()
{
    init();
}
void ayikOption::init()
{
}
void ayikOption::postInit()
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
    rating = new ayikRating(false,true,1,5);
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
    lineedit_word_filter = new QLineEdit();
    tableview = new TableView();

    tableview->model->setHeaders(tableHeaders);
    tableview->getView()->setColumnHidden(3,true);

    txt_meaning = new QTextEdit();
    loadWordsTable();
    QHBoxLayout *wwbtnlayout = new QHBoxLayout();
    wwbtnlayout->addWidget(btn_add_file);
    wwbtnlayout->addWidget(btn_delete_word);
    wwbtnlayout->addStretch();

    QHBoxLayout *wwfilterlayout = new QHBoxLayout();
    wwfilterlayout->addWidget(new QLabel(tr("Filter:")));
    wwfilterlayout->addWidget(lineedit_word_filter);
    wwbtnlayout->addStretch();

    wwlayout->addLayout(wwbtnlayout);
    wwlayout->addLayout(wwfilterlayout);
    QSplitter *word_meaning_splitter = new QSplitter(Qt::Vertical,this);
    word_meaning_splitter->addWidget(tableview->getView());
    word_meaning_splitter->addWidget(txt_meaning);

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
ayikOption* ayikOption::getInstance()
{
    if(instance==NULL) instance = new ayikOption();
    return instance;
}
int ayikOption::getMessageFrequency() {
    QString siklik = cmb_freq->currentText();
    return siklik.toInt();
}
QString ayikOption::getTag()
{
    QString tag="";
    /*if(cmb_tag->currentIndex()>0) {
        tag = cmb_tag->currentText();
    }*/
    tag = lineedit_tag->text();
    if(tag == tagAllWords) tag="";
    return tag;
}
QString ayikOption::getRating()
{
    return rating->getRating();
}
QString ayikOption::getRatingOperator()
{
    QString ratingoper = cmb_rating_oper->currentText();
    return ratingoper;
}
void ayikOption::createActions()
{
    connect(btn_add_file,SIGNAL(clicked()),this,SLOT(slot_add_file()));
    connect(btn_delete_word,SIGNAL(clicked()),this,SLOT(slot_delete_word()));
    //connect(cmb_tag,SIGNAL(clicked()),this,SLOT(slot_add_file()));//todo
    connect(tableview->getView()->selectionModel(),SIGNAL(currentRowChanged(const QModelIndex&,const QModelIndex&)),this,SLOT(slot_word_selected(const QModelIndex&,const QModelIndex&)));
    connect(txt_meaning,SIGNAL(textChanged()),this,SLOT(slot_txt_meaning_changed()));
    connect(lineedit_word_filter,SIGNAL(textChanged(const QString&)),this,SLOT(slot_filterTextChanged(const QString&)));
    connect(btn_tag_select,SIGNAL(clicked()),this,SLOT(slot_chooseTag()));
    connect(tagslist,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slot_tagChosen(QListWidgetItem*)));

}
void ayikOption::closeEvent(QCloseEvent *event)
{
    //todo: save & load
    event->ignore();
    //    loadCheckedFileList();
    hide();
}
void ayikOption::loadWordsTable()
{

    tableview->model->clear();
    tableview->model->setHeaders(tableHeaders);

    QString err;
    QueryResult res;
    ayikDB *db = ayikDB::getInstance();
    db->getWordList(res,err);
    if(err != "") qDebug()<<err;

    for(int i=0;i<res.records.size();i++) {
        tableview->model->insertRows(0, 1, QModelIndex());
        QList<QString> row = res.records.at(i);
        for(int j=0;j<row.size();j++) {
            QModelIndex index = tableview->model->index(0, j, QModelIndex());
            tableview->model->setData(index, row.at(j), Qt::EditRole);
        }
    }
}

void ayikOption::slot_add_file()
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
                ayikDB::getInstance()->addWords(fileName,tag,rating);
                loadWordsTable();//reload list
            }
        }
    }
}

void ayikOption::slot_delete_word()
{
    QString err;
    QModelIndexList selection = tableview->getView()->selectionModel()->selectedRows(1);
    QProgressDialog pd(tr("Deleting.."), tr("Cancel"), 0, selection.size());
    pd.setWindowModality(Qt::WindowModal);
    int steps=0;
    QList<ayikWord> wordlist;
    for(int i=selection.size() - 1;i >=0 ;i--) {
        if (pd.wasCanceled()) break;
        QModelIndex proxyIndex = selection.at(i);
        QModelIndex index = tableview->proxyModel->mapToSource(proxyIndex);

        QString name = tableview->model->data(index,Qt::EditRole).toString();
        ayikWord w;
        w.setName(name);
        wordlist.append(w);
        if(wordlist.size()>50) {
            ayikDB::getInstance()->deleteWords(wordlist,err);
            steps+=wordlist.size();
            wordlist.clear();
            pd.setValue(steps);
        }
    }
    if(wordlist.size()>0) {
        ayikDB::getInstance()->deleteWords(wordlist,err);
        steps+=wordlist.size();
        wordlist.clear();
        pd.setValue(steps);
    }
    loadWordsTable();//reload list
}

void ayikOption::slot_word_selected(const QModelIndex &current,const QModelIndex &previous)
{
    QModelIndex currIndex = tableview->proxyModel->mapToSource(current);
    QModelIndex prevIndex = tableview->proxyModel->mapToSource(previous);
    if(prevIndex.isValid() && txt_meaning_changed) {
        txt_meaning_changed=false;
        QString prevwordname = tableview->model->getValue(prevIndex.row(),1);
        ayikWord prevword;
        QString preverr;
        ayikDB::getInstance()->getWordMemdb(prevword,prevwordname,preverr);
        if(txt_meaning->toPlainText().isEmpty()) prevword.setMeaning("");
        else prevword.setMeaning(txt_meaning->toHtml());
        ayikDB::getInstance()->updateWord(prevword);
        tableview->model->setValue(previous.row(),3,prevword.getMeaning());
    }
    if(currIndex.isValid()) {
        QString currwordname = tableview->model->getValue(currIndex.row(),1);
        QString currwordmeaning = tableview->model->getValue(currIndex.row(),3);
        ayikWord currword;
        QString currerr;
        ayikDB::getInstance()->getWordMemdb(currword,currwordname,currerr);
        currwordmeaning = currword.getMeaning();
        txt_meaning->setHtml(currwordmeaning);
    }
}
void ayikOption::slot_txt_meaning_changed()
{
    txt_meaning_changed=true;
}
void ayikOption::slot_filterTextChanged(const QString& text)
{
    tableview->proxyModel->setFilterWildcard(text);
}

void ayikOption::showHelp()
{
    tabs->setCurrentIndex(2);
    setVisible(true);
}
void ayikOption::slot_chooseTag()
{
    reloadTagList();
    tagslist->setVisible(true);
}
void ayikOption::reloadTagList()
{
    QStringList items_tag;
    QString err;
    ayikDB::getInstance()->getTagList(items_tag,err);//todo: err check
    items_tag.push_front(tagAllWords);
    tagslist->clear();
    tagslist->addItems(items_tag);
}
void ayikOption::slot_tagChosen(QListWidgetItem * item)
{
    lineedit_tag->setText(item->text());
    tagslist->setVisible(false);
}
