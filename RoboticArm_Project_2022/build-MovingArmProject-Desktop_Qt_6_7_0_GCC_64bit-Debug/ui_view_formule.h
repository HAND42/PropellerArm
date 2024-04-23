/********************************************************************************
** Form generated from reading UI file 'view_formule.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEW_FORMULE_H
#define UI_VIEW_FORMULE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_view_formule
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *titre_horizontalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *titre_label;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *formule_horizontalLayout;
    QLabel *formule_label;
    QLineEdit *formule_lineEdit;
    QHBoxLayout *valider_horizontalLayout;
    QPushButton *verifier_la_formule_pushButton;
    QPushButton *tester_la_formule_pushButton;
    QPushButton *formule_info_pushButton;
    QHBoxLayout *erreur_horizontalLayout;
    QLabel *erreur_titre_label;
    QLabel *erreur_label;

    void setupUi(QMainWindow *view_formule)
    {
        if (view_formule->objectName().isEmpty())
            view_formule->setObjectName("view_formule");
        view_formule->setWindowModality(Qt::NonModal);
        view_formule->resize(800, 179);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(view_formule->sizePolicy().hasHeightForWidth());
        view_formule->setSizePolicy(sizePolicy);
        view_formule->setMinimumSize(QSize(800, 179));
        view_formule->setMaximumSize(QSize(800, 179));
        view_formule->setFocusPolicy(Qt::NoFocus);
        centralWidget = new QWidget(view_formule);
        centralWidget->setObjectName("centralWidget");
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        titre_horizontalLayout = new QHBoxLayout();
        titre_horizontalLayout->setSpacing(6);
        titre_horizontalLayout->setObjectName("titre_horizontalLayout");
        titre_horizontalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        verticalSpacer = new QSpacerItem(10, 60, QSizePolicy::Minimum, QSizePolicy::Fixed);

        titre_horizontalLayout->addItem(verticalSpacer);

        titre_label = new QLabel(centralWidget);
        titre_label->setObjectName("titre_label");
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(titre_label->sizePolicy().hasHeightForWidth());
        titre_label->setSizePolicy(sizePolicy1);
        titre_label->setMaximumSize(QSize(300, 50));
        titre_label->setFrameShape(QFrame::Box);
        titre_label->setAlignment(Qt::AlignCenter);

        titre_horizontalLayout->addWidget(titre_label);

        verticalSpacer_2 = new QSpacerItem(10, 60, QSizePolicy::Minimum, QSizePolicy::Fixed);

        titre_horizontalLayout->addItem(verticalSpacer_2);


        verticalLayout->addLayout(titre_horizontalLayout);

        formule_horizontalLayout = new QHBoxLayout();
        formule_horizontalLayout->setSpacing(6);
        formule_horizontalLayout->setObjectName("formule_horizontalLayout");
        formule_horizontalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        formule_label = new QLabel(centralWidget);
        formule_label->setObjectName("formule_label");
        formule_label->setMaximumSize(QSize(61, 25));

        formule_horizontalLayout->addWidget(formule_label);

        formule_lineEdit = new QLineEdit(centralWidget);
        formule_lineEdit->setObjectName("formule_lineEdit");

        formule_horizontalLayout->addWidget(formule_lineEdit);


        verticalLayout->addLayout(formule_horizontalLayout);

        valider_horizontalLayout = new QHBoxLayout();
        valider_horizontalLayout->setSpacing(6);
        valider_horizontalLayout->setObjectName("valider_horizontalLayout");
        valider_horizontalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        verifier_la_formule_pushButton = new QPushButton(centralWidget);
        verifier_la_formule_pushButton->setObjectName("verifier_la_formule_pushButton");
        verifier_la_formule_pushButton->setEnabled(false);
        sizePolicy.setHeightForWidth(verifier_la_formule_pushButton->sizePolicy().hasHeightForWidth());
        verifier_la_formule_pushButton->setSizePolicy(sizePolicy);
        verifier_la_formule_pushButton->setMaximumSize(QSize(150, 16777215));
        verifier_la_formule_pushButton->setFocusPolicy(Qt::StrongFocus);

        valider_horizontalLayout->addWidget(verifier_la_formule_pushButton);

        tester_la_formule_pushButton = new QPushButton(centralWidget);
        tester_la_formule_pushButton->setObjectName("tester_la_formule_pushButton");
        tester_la_formule_pushButton->setEnabled(false);
        sizePolicy.setHeightForWidth(tester_la_formule_pushButton->sizePolicy().hasHeightForWidth());
        tester_la_formule_pushButton->setSizePolicy(sizePolicy);
        tester_la_formule_pushButton->setMinimumSize(QSize(0, 0));
        tester_la_formule_pushButton->setMaximumSize(QSize(150, 16777215));

        valider_horizontalLayout->addWidget(tester_la_formule_pushButton);

        formule_info_pushButton = new QPushButton(centralWidget);
        formule_info_pushButton->setObjectName("formule_info_pushButton");
        sizePolicy.setHeightForWidth(formule_info_pushButton->sizePolicy().hasHeightForWidth());
        formule_info_pushButton->setSizePolicy(sizePolicy);
        formule_info_pushButton->setMaximumSize(QSize(150, 16777215));

        valider_horizontalLayout->addWidget(formule_info_pushButton);


        verticalLayout->addLayout(valider_horizontalLayout);

        erreur_horizontalLayout = new QHBoxLayout();
        erreur_horizontalLayout->setSpacing(6);
        erreur_horizontalLayout->setObjectName("erreur_horizontalLayout");
        erreur_horizontalLayout->setSizeConstraint(QLayout::SetNoConstraint);
        erreur_titre_label = new QLabel(centralWidget);
        erreur_titre_label->setObjectName("erreur_titre_label");
        erreur_titre_label->setEnabled(true);
        erreur_titre_label->setMaximumSize(QSize(61, 25));

        erreur_horizontalLayout->addWidget(erreur_titre_label);

        erreur_label = new QLabel(centralWidget);
        erreur_label->setObjectName("erreur_label");
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(erreur_label->sizePolicy().hasHeightForWidth());
        erreur_label->setSizePolicy(sizePolicy2);
        erreur_label->setMinimumSize(QSize(0, 25));
        erreur_label->setMaximumSize(QSize(16777215, 25));

        erreur_horizontalLayout->addWidget(erreur_label);


        verticalLayout->addLayout(erreur_horizontalLayout);

        view_formule->setCentralWidget(centralWidget);
        QWidget::setTabOrder(formule_lineEdit, verifier_la_formule_pushButton);
        QWidget::setTabOrder(verifier_la_formule_pushButton, tester_la_formule_pushButton);
        QWidget::setTabOrder(tester_la_formule_pushButton, formule_info_pushButton);

        retranslateUi(view_formule);

        QMetaObject::connectSlotsByName(view_formule);
    } // setupUi

    void retranslateUi(QMainWindow *view_formule)
    {
        view_formule->setWindowTitle(QCoreApplication::translate("view_formule", "FORMULE", nullptr));
        titre_label->setText(QCoreApplication::translate("view_formule", "FORMULE POUR CALCULER THRUST", nullptr));
        formule_label->setText(QCoreApplication::translate("view_formule", "Formule:", nullptr));
        verifier_la_formule_pushButton->setText(QCoreApplication::translate("view_formule", "VERIFIER LA FORMULE", nullptr));
        tester_la_formule_pushButton->setText(QCoreApplication::translate("view_formule", "TESTER LA FORMULE", nullptr));
        formule_info_pushButton->setText(QCoreApplication::translate("view_formule", "Informations", nullptr));
        erreur_titre_label->setText(QCoreApplication::translate("view_formule", "ERREUR:", nullptr));
        erreur_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class view_formule: public Ui_view_formule {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEW_FORMULE_H
