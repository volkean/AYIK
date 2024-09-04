#ifndef AYIKOPTION_H
#define AYIKOPTION_H

#include <QtGui>
#include "starrating.h"
#include "ayikrating.h"
#include "ayikdb.h"
#include "tableview.h"
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QMainWindow>

class AyikOption : public QMainWindow
{
    Q_OBJECT

public:
    AyikOption();
    static AyikOption* getInstance();
    void init();
    void postInit();

    int getMessageFrequency();
    QString getTag();
    QString getRating();
    QString getRatingOperator();

    void showHelp();

private:
    static AyikOption* instance;

    QTabWidget* tabs;

    QGroupBox* group_freq;
    QStringList freqLevels;
    QComboBox* cmb_freq;

    QGroupBox* group_tag;
    QString tagAllWords;
    QLineEdit *lineedit_tag;
    QListWidget *tagslist;
    QPushButton *btn_tag_select;

    QGroupBox* group_src_hist;
    QStringList ratingLevels;
    AyikRating* rating;
    QComboBox *cmb_rating_oper;

    QPushButton* btn_add_file;
    QPushButton* btn_delete_word;
    QPushButton* btn_delete_all;
    QPushButton* btn_update_meaning;
    QLineEdit* lineedit_word_filter;
    QStringList tableHeaders;
    TableView* tableview;

    QTextEdit* txt_meaning;
    bool txt_meaning_changed;
    AyikWord currword;

    QSqlDatabase db;

private:
    void createActions();
    void reloadTagList();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void slot_add_file();
    void slot_delete_word();
    void slot_delete_all();
    void slot_update_meaning();
    void slot_word_selected(const QModelIndex &current,const QModelIndex &previous);
    void slot_txt_meaning_changed();
    void slot_filterTextChanged(const QString& text);
    void slot_chooseTag();
    void slot_tagChosen(QListWidgetItem * item);
};

#endif // AYIKOPTION_H
