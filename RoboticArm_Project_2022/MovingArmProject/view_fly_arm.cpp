#include "view_fly_arm.h"
#include "ui_view_fly_arm.h"

view_fly_arm::view_fly_arm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::view_fly_arm)
{
    ui->setupUi(this);

    // INIT
    this->objects_init();
    this->sample_time_init();
    this->graph_init();
    this->attributs_init();
    this->widgets_hide();

    connect(&this->myView_setting_PC_controller, SIGNAL(modification_valeurs()), this, SLOT(update_controller_type()));

    connect(&this->myView_setting_sample_time, SIGNAL(modification_valeurs()), this, SLOT(update_graphs_sample_time_timer()));

    // FORMULE
    connect(this->myView_formule, SIGNAL(view_formule_closed()), this, SLOT(view_formule_closed()));

    connect(this->myView_formule, SIGNAL(tester_la_formule_pushButton_clicked()), this, SLOT(slot_formule_a_tester()));

    connect(this->myView_formule, SIGNAL(formule_en_cours_de_modification()), this, SLOT(view_formule_modification()));
}

view_fly_arm::~view_fly_arm()
{
    this->myModel_file_setting->file_write();

    delete ui;
}

void view_fly_arm::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)

//    qDebug() << "resize event";
    this->ui->view_panel->resize(this->ui->groupBox_6->width(), this->ui->groupBox_6->height());
    this->ui->terminal_groupBox->resize(this->ui->terminal_panel->width() - 320, this->ui->terminal_groupBox->height());

    this->nouvelles_dimensions = this->ui->graph_theta_temps_legend_label->geometry();
//    qDebug() << "label dimensions = " << this->nouvelles_dimensions;
    this->nouvelles_dimensions.setTop(this->ui->graph_theta_legend->height() - HAUTEUR_A_ENLEVER);
//    qDebug() << "graph dimensions = " << this->ui->graph_theta_legend->geometry();
    this->ui->graph_theta_temps_legend_label->setGeometry(this->nouvelles_dimensions);
//    qDebug() << "graph_theta_temps_legend_label = " << this->ui->graph_theta_temps_legend_label->geometry();

    this->nouvelles_dimensions = this->ui->graph_thrust_temps_legend_label->geometry();
    this->nouvelles_dimensions.setTop(this->ui->graph_thrust_legend->height() - HAUTEUR_A_ENLEVER);
    this->ui->graph_thrust_temps_legend_label->setGeometry(this->nouvelles_dimensions);

    this->nouvelles_dimensions = this->ui->graph_theta_dot_temps_legend_label->geometry();
    this->nouvelles_dimensions.setTop(this->ui->graph_theta_dot_legend->height() - HAUTEUR_A_ENLEVER);
    this->ui->graph_theta_dot_temps_legend_label->setGeometry(this->nouvelles_dimensions);

    this->nouvelles_dimensions = this->ui->graph_theta_dotdot_temps_legend_label->geometry();
    this->nouvelles_dimensions.setTop(this->ui->graph_theta_dotdot_legend->height() - HAUTEUR_A_ENLEVER);
    this->ui->graph_theta_dotdot_temps_legend_label->setGeometry(this->nouvelles_dimensions);

    this->myDraw->adapte_la_taille(this->ui->view_panel->size());
}

// ------------------------------------------------------
//                      TIMER
// ------------------------------------------------------
//
// timerEvent : GD: period of execution is specified in Sample time Settings see in GUI
//
void view_fly_arm::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    // données à enregistrer
    this->variables_valeurs_a_enregistrer(DONNEES_DEBUT);
    //        qDebug() << "tick_compteur: " << this->tick_compteur;
    //        qDebug() << "tick_max: " << this->tick_max;
    this->tick_compteur++;
    //        qDebug() << "tick_compteur: " << this->tick_compteur;

    // données à enregistrer
    this->variables_valeurs_a_enregistrer(DONNEES_TIMER);

    // incrémenter les compteurs
    this->simulator_step_count++;
    this->controller_step_count++;
    this->graph_step_count++;

//    this->temps_execution_theorique += this->step;
    //        qDebug() << "temps_execution_theorique = " << this->temps_execution_theorique;

    //
    if(this->myHilsModeSerialCommunicator->getHilsMode() != HILS_MODE_MANUAL_THRUST_COMMAND)
    {
        //            qDebug() << "1";
        this->myThreadSimulatorController->hils_mode_execute(); //see threadSimulatorController.cpp
    }

    // controller: GD: gets executed after some step counts (simulator faster than controller
    if(this->controller_step_count == this->controller_step && this->myHilsModeSerialCommunicator->getHilsMode() != HILS_MODE_MANUAL_THRUST_COMMAND
        && this->myHilsModeSerialCommunicator->getHilsMode() != HILS_MODE_1_)
    {
        //            qDebug() << "2";
        if(!this->formule_active)
            this->myThreadSimulatorController->runController();
        else
        {
//            qDebug() << "\tformule_active";
            this->formule_propThrustcmd = this->myModel_formule->calculer_le_resultat(this->formule_valeurs);

            if (this->formule_propThrustcmd > MAX_THRUST)
                this->formule_propThrustcmd = MAX_THRUST;
            else if (this->formule_propThrustcmd < MIN_THRUST)
                this->formule_propThrustcmd = MIN_THRUST;

            this->myArmPropController->SetThrustCmd(this->formule_propThrustcmd);
        }

        // données à enregistrer
        this->variables_valeurs_a_enregistrer(DONNEES_ARMPROPCONTROLLER);

        this->controller_step_count = 0;
    }

    //==>> partie à modifier selon les indications de Guillaume
    // simulator
    //if(this->myHilsModeSerialCommunicator->getHilsMode() != HILS_MODE_REAL_ANGLE && this->simulator_step_count == this->simulator_step)
    if(this->simulator_step_count == this->simulator_step && this->myHilsModeSerialCommunicator->getHilsMode() != HILS_MODE_REAL_ANGLE && this->myHilsModeSerialCommunicator->getHilsMode() != HILS_MODE_1_)
    {
        //            qDebug() << "3";
        this->myThreadSimulatorController->runSimulator();

        // données à enregistrer
        this->variables_valeurs_a_enregistrer(DONNEES_ARMPROPSIMULATOR);

        this->simulator_step_count = 0;
    }

    // données à enregistrer
    this->variables_valeurs_a_enregistrer(DONNEES_GRAPH_1);

    // graph
    if(this->graph_step_count == this->graph_step)
    {
        //            qDebug() << "4";
        this->graph_step_count = 0;
        this->timer_graph += this->timer_graph_step;
        //            qDebug() << "this->timer_graph = " << this->timer_graph;

        this->graphs_draw();

        // données à enregistrer
        this->variables_valeurs_a_enregistrer(DONNEES_GRAPH_2);

        this->console_info_update();

        //            qDebug() << "duree en fin de graphique (en millisecondes) = " << this->tick_duree_totale.elapsed();
    }
// =======================================================
// DEBUT PARTIE POUR RALENTIR LE TIMER
//    this->temps_execution_reel = this->tick_duree_totale.elapsed();
//    qDebug() << "temps_execution_reel = " << this->temps_execution_reel;

//    this->timer_en_avance = this->temps_execution_theorique - this->temps_execution_reel + this->temps_a_rattraper;
    //        qDebug() << "timer_en_avance = " << this->timer_en_avance;

    // this->timer_en_avance > 0: temps = 3093
    // this->timer_en_avance > this->step: temps = 3077
    // this->timer_en_avance > 0: temps = 3093
//    if(this->timer_en_avance > this->step)
//    {
//        //            qDebug() << "ralentir le timer";
//        QThread::msleep(static_cast<unsigned long>(10));
//    }

    //        qDebug() << "temps_execution_theorique = " << this->temps_execution_theorique;
    //        qDebug() << "temps_execution_reel = " << this->temps_execution_reel;
// DEBUT PARTIE POUR RALENTIR LE TIMER
// =======================================================
    // données à enregistrer
    this->variables_valeurs_a_enregistrer(DONNEES_FIN);

    if (this->tick_compteur == this->tick_max)
    {
        this->timer1->stop();
//        qDebug() << "temps théorique (en millisecondes) = " << this->temps_execution_theorique;
//        qDebug() << "this->tick_duree_totale (en millisecondes) = " << this->tick_duree_totale.elapsed();
//        qDebug() << "this->tick_compteur = " << this->tick_compteur;
        this->myThreadSimulatorController_is_create =false;

        this->myArmPropSimulator->init();
        this->myArmPropController->init();

        this->buttons_enabled(false, false, true);

        // données à afficher
//        QStringList variables_valeurs_enregistrees;
//        for(int index = 0; index < this->variables_valeurs_enregistrees.length(); index++)
//            qDebug() << this->variables_valeurs_enregistrees.at(index);

//        QString variables_valeurs_enregistrees;
//        qDebug() << this->variables_valeurs_enregistrees;
    }
}

void view_fly_arm::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)

    // qDebug() << "view_formule::closeEvent";
    if(this->myView_formule->isVisible())
        this->myView_formule->close();
}

// ------------------------------------------------------
//                      MENU FILE: EXIT + SAVE
// ------------------------------------------------------
void view_fly_arm::on_actionSave_triggered()
{
    QPixmap image = this->graph_theta.donne_le_qchartview()->grab();
    image.save("..\\Theta-Time.png", "PNG");

    image = this->graph_thrust.donne_le_qchartview()->grab();
    image.save("..\\Thrust-Time.png", "PNG");

    image = this->graph_theta_dot.donne_le_qchartview()->grab();
    image.save("..\\Theta_dot-Time.png", "PNG");

    image = this->graph_theta_dotdot.donne_le_qchartview()->grab();
    image.save("..\\Theta_dotdot-Time.png", "PNG");
}

void view_fly_arm::on_actionSave_Data_triggered()
{
    if(!this->variables_valeurs_enregistrees.isEmpty())
    {
        QString fichier_nom;
        QDateTime date_heure = QDateTime::currentDateTime();

        //    qDebug() << "date =" << date_heure.toString("yyyy-MM-dd_HH-mm");
        fichier_nom.append("..\\save_data_");
        fichier_nom.append(date_heure.toString("yyyy-MM-dd_HH-mm"));
        fichier_nom.append(".txt");
//        qDebug() << "fichier =" << fichier_nom;

        QFile fichier(fichier_nom);

        if (!fichier.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << QString::fromUtf8("le fichier n'a pas été trouvé");
        }
        else
        {
            QTextStream out(&fichier);
            out << this->variables_valeurs_enregistrees;
            fichier.close();
            qDebug() << QString::fromUtf8("le fichier a été écrit");
        }
    }
}

void view_fly_arm::on_actionExit_triggered()
{
    this->close();
}

// ------------------------------------------------------
//                      MENU MODE
// ------------------------------------------------------
void view_fly_arm::on_actionPC_Controleur_triggered()
{
    this->hils_mode_choisi = true;

    this->theta_desired = true;
    this->ui->theta_OR_thrust_desired_label->setText("Desired Theta:");
    this->ui->theta_desired_unite_label->setText("deg");
    this->ui->theta_OR_thrust_desired_value_label->setText("0");
    this->ui->theta_OR_thrust_desired_trackBar->setValue(0);
    this->ui->theta_OR_thrust_desired_trackBar->setMaximum(179);

//    this->myView_setting_PC_controller.show();

    this->update_controller_type();
}

void view_fly_arm::on_actionHils_mode_1_triggered()
{
    this->hils_mode_choisi = true;

    this->modes_infos_communes(false, true, true, false, "Mode: Simulateur PC", HILS_MODE_1_);
}

void view_fly_arm::on_actionPC_Display_Real_angle_triggered()
{
    //this->modes_infos_communes(sender, e, false, true, true, false, L"Mode: PC display real angle", HILS_MODE_2);
    this->hils_mode_choisi = true;

    this->modes_infos_communes(false, true, true, false, "Mode: PC display real angle", HILS_MODE_REAL_ANGLE);
}

void view_fly_arm::on_actionHils_mode_3_triggered()
{
    this->hils_mode_choisi = true;

    this->modes_infos_communes(false, true, true, false, "Mode: HILS MODE 3", HILS_MODE_3_);
}

void view_fly_arm::on_actionDemo_Manuel_Thrust_Command_triggered()
{
    this->theta_desired = false; // THRUST DESIRED
    this->ui->theta_OR_thrust_desired_label->setText("Desired Thrust:");
    this->ui->theta_desired_unite_label->setText("N");
    this->ui->theta_OR_thrust_desired_value_label->setText("0");
    this->ui->theta_OR_thrust_desired_trackBar->setValue(0);
    this->ui->theta_OR_thrust_desired_trackBar->setMaximum(30);

    this->hils_mode_choisi = true;

    this->modes_infos_communes(true, false, false, true, "Mode: Demo Manuel Thrust", HILS_MODE_MANUAL_THRUST_COMMAND);
}

void view_fly_arm::on_actionFormule_Calcul_Thrust_triggered()
{
    this->hils_mode_choisi = true;

    this->formule_active = true;

    this->theta_desired = true;
    this->ui->theta_OR_thrust_desired_label->setText("Desired Theta:");
    this->ui->theta_desired_unite_label->setText("deg");
    this->ui->theta_OR_thrust_desired_value_label->setText("0");
    this->ui->theta_OR_thrust_desired_trackBar->setValue(0);
    this->ui->theta_OR_thrust_desired_trackBar->setMaximum(179);

    this->update_controller_type();

    this->buttons_enabled(false, false, false);
    this->ui->mode_label->setText("Formula for calculating Thrust");

    // mettre les boutons 'PLAY' 'PAUSE' et 'STOP' avec les valeurs de départ

    this->myView_formule->show();
}

// ------------------------------------------------------
//                      MENU SETTING: SAMPLE TIME + PC CONTROLLER
// ------------------------------------------------------
void view_fly_arm::on_actionSample_time_Configuration_triggered()
{
    if(this->ui->stop_button->isEnabled() == true)
    {
        this->on_stop_button_clicked();
    }

    this->myView_setting_sample_time.show();
}

void view_fly_arm::on_actionPC_controller_Configuration_triggered()
{
//    qDebug() << "1 - afficher myView_setting_PC_controller";
    this->myView_setting_PC_controller.show();

//    qDebug() << "this->controller_type = " << this->myModel_setting_PC_controller->controller_type_string_get();

//    qDebug() << "theta_desired = " << this->theta_desired;
    if(this->ui->theta_OR_thrust_desired_trackBar->isVisible() == true)
    {
        if(this->theta_desired == true)
        {
//            qDebug() << "modification dans: myView_setting_PC_controller";
            if(this->myModel_setting_PC_controller->controller_type_get() == CASCADE_CONTROLLER)
            {
//                qDebug() << "Mode: PC control (Cascade)";
                this->ui->mode_label->setText("Mode: PC control (Cascade)");
            }
            else // LEADLAG_CONTROLLER
            {
//                qDebug() << "Mode: PC control (Leadlag)";
                this->ui->mode_label->setText("Mode: PC control (Leadlag)");
            }
        }
    }
}

// ------------------------------------------------------
//                      MENU HELP: ABOUT + REPAIR
// ------------------------------------------------------
void view_fly_arm::on_actionAbout_triggered()
{
    this->myView_about.show();
}

void view_fly_arm::on_actionRepair_file_setting_txt_triggered()
{
    this->myModel_setting_PC_controller->init_values();
    this->my_model_setting_sample_time->init();
    this->myTime->init();
    this->myModel_file_setting->file_write();
}
// ------------------------------------------------------
//                      PANEL MODE
// ------------------------------------------------------

void view_fly_arm::on_play_button_clicked()
{
    // CONFIGURATION ACTUELLE POUR UTILISER EN MODE PC_CONTROLLER
//    qDebug() << "play-theta_desired = " << QString::number(this->myArmPropController->GetThetaCmd());

    if(    this->hils_mode_choisi)
    {
        // effacer les graphiques si je ne suis pas en mode pause
        if(this->ui->pause_button->isEnabled() == false && this->ui->stop_button->isEnabled() == false)
        {
            this->graphs_clear();
        }
        this->buttons_enabled(false, false, true); // prevents from clicking twice on play ince ativated but the other get accessible

        // initialiser les variables pour l'ajustement du timer
        this->timer_ajuster_init();

        if(this->myThreadSimulatorController_is_create == false) // to not create a second class
        {
            this->myThreadSimulatorController_is_create = true; // create class
            this->myThreadSimulatorController->init();
            this->tick_compteur = 0;

            // enregistrer les données
            this->variables_valeurs_a_enregistrer_initialisation();

            //this->tick_duree_totale.start();                                   GHRUJTEFRKSDCQLXJKGDRTHYFBNKV?L.CXSKJ.DGTHY?BRFNGVCXWJRFGKTBHNDVKSC?XW>
//            qDebug() << "start_PreciseTimer";
            this->timer1->start(this->step, Qt::PreciseTimer, this);

//            qDebug() << "start_CoarseTimer";
//            this->timer1->start(this->step, Qt::CoarseTimer, this);

//            qDebug() << "start_VeryCoarseTimer";
//            this->timer1->start(this->step, Qt::VeryCoarseTimer, this);
        }
    }
}

void view_fly_arm::on_pause_button_clicked()
{
    if(    this->hils_mode_choisi)
    {
        this->buttons_enabled(true, false, true);
        if(this->myThreadSimulatorController_is_create == true)
        {
            this->timer1->stop();
            this->myThreadSimulatorController_is_create = false;
        }
    }
}

void view_fly_arm::on_stop_button_clicked()
{
    if(    this->hils_mode_choisi)
    {
        this->buttons_enabled(true, false, false);

        if(this->myThreadSimulatorController_is_create == true)
        {
            this->timer1->stop();
            this->myThreadSimulatorController_is_create =false;
        }

        this->myArmPropSimulator->init();
        this->myArmPropController->init();

        // mettre à jour ThetaCmd ou ThrustCmd
        if(this->ui->theta_OR_thrust_desired_trackBar->isVisible())
            this->theta_OR_thrust_desired_save(this->ui->theta_OR_thrust_desired_trackBar->value());

        this->graphs_clear();
        // pour avoir en abscisse une graduation de 0 à 2
        this->timer_graph = 0;
        this->graphs_draw();
    }
}

void view_fly_arm::on_theta_OR_thrust_desired_trackBar_valueChanged(int value)
{
//    qDebug() << "on_theta_OR_thrust_desired_trackBar_valueChanged = " << value;
    this->theta_OR_thrust_desired_save(value);
}

void view_fly_arm::on_time_textBox_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1)

    int temp;
    temp = this->ui->time_textBox->text().toInt();
    if (temp > 9999999)
    {
        temp = 9999999;
        this->ui->time_textBox->setText(QString::number(temp));
    }
    this->time_desired = temp;
    this->tick_max = this->time_desired * 1000 / this->step;
    this->myTime->time_desired_set(temp);
}
// ------------------------------------------------------
//                      CONSOLE
// ------------------------------------------------------
void view_fly_arm::console_info_update(void)
{
    // si la console est active
    if (this->ui->theta_OR_thrust_desired_trackBar->isVisible() == false)
    {
        this->signalValue = this->myHilsModeSerialCommunicator->getSignal();
        this->ui->terminal_signal_value_label->setText("Current Signal:" + QString::number(this->signalValue));
        this->ui->signal_value_progressBar->setValue(signalValue);
    }
}

void view_fly_arm::on_connect_button_clicked()
{
    //this->label2->Text = "";
    //-------------Connect serialcommunication-------------------
    //verify that the Setting PORT USB or COM and BAUD RATE are selected
    //
    QString string_temp;
    // make sure port isn't open
    qDebug()<< "Pressed";
    if(this->myHilsModeSerialCommunicator->myCommunicator->portSuccessfullyOpened() == false)
    {
        qDebug()<< "set up port";

        if(this->ui->usb_port_radioButton->isChecked())
        {
            qDebug()<< "type usb";

            // essaie d'ouvrir la connection au port usb
            this->myHilsModeSerialCommunicator->myCommunicator->open_port(this->ui->baud_rate_comboBox->currentText().toInt(), 0);
            string_temp = this->myHilsModeSerialCommunicator->getMessage_serial_communicator();
            this->ui->terminal_info_label->setText(string_temp);
        }
        else // com_port_radioButton
        {
            qDebug()<< "type com";

            // essaie d'ouvrir la connection au port com
            this->myHilsModeSerialCommunicator->myCommunicator->open_port(this->ui->baud_rate_comboBox->currentText().toInt(), this->ui->com_port_comboBox->currentText().toInt());
            string_temp = this->myHilsModeSerialCommunicator->getMessage_serial_communicator();
            this->ui->terminal_info_label->setText(string_temp);
        }
    }
    qDebug()<< "fin part 1";

    // vérifier que le port est ouvert
    if(this->myHilsModeSerialCommunicator->myCommunicator->portSuccessfullyOpened() == false)
    {
        qDebug()<< "pas bien fait part 1";

        if(this->ui->usb_port_radioButton->isChecked())
        {
            string_temp = this->myHilsModeSerialCommunicator->getMessage_serial_communicator();
            string_temp.append("Connect cable USB and press Connect ...");
            this->ui->terminal_info_label->setText(string_temp);
        }
        else // com_port_radioButton
        {
            string_temp = this->myHilsModeSerialCommunicator->getMessage_serial_communicator();
            string_temp.append("Connect cable COM and press Connect ...");
            this->ui->terminal_info_label->setText(string_temp);
        }
    }
    else // SI le port est ouvert
    {
        qDebug()<< "ok part 1";

        this->connect_disconnect_true_or_false(false);

        //this->myHilsModeSerialCommunicator->init();
        this->myHilsModeSerialCommunicator->run();

        // écrire les messages d'information
        switch(this->myHilsModeSerialCommunicator->getMessage())
        {
            case 1:
                string_temp.append("Could not open port!\nExiting...\n");
                this->ui->terminal_info_label->setText(string_temp);
                break;
            case 2:
                string_temp.append("Connecting to the board ... \n");
                string_temp.append("Failed to switch into HILS mode ");
                string_temp.append(QString::number(this->myHilsModeSerialCommunicator->getHilsMode()));
                string_temp.append(" - exiting...\n");
                this->ui->terminal_info_label->setText(string_temp);
                break;
            case 3:
                string_temp.append("Connecting to the board ... \n");
                string_temp.append("Successfully switched into HILS mode ");
                string_temp.append(QString::number(this->myHilsModeSerialCommunicator->getHilsMode()));
                string_temp.append(".\n");
                this->ui->terminal_info_label->setText(string_temp);
                break;
        }
    }
}

void view_fly_arm::on_disconnect_button_clicked()
{
    this->disconnect_method();
}

void view_fly_arm::disconnect_method(void)
{
    //-------------Disconnect serialcommunication-------------------
    this->timer1->stop();
    this->myHilsModeSerialCommunicator->stop();
    this->ui->terminal_info_label->setText("Press Connect ...");
    this->ui->terminal_signal_value_label->setText("");
    this->ui->signal_value_progressBar->setValue(0);

    this->connect_disconnect_true_or_false(true);
}
// ------------------------------------------------------
//                      GRAPHS
// ------------------------------------------------------
void view_fly_arm::graphs_draw(void)
{
    qDebug() <<"update frame";
    QString string_temp = this->myHilsModeSerialCommunicator->update_angle();
    this->ui->terminal_info_label->setText(string_temp);
    this->rad_arm = this->myArmPropSimulator->GetTheta();
    this->degree_arm = this->rad_arm * 180.0 / MY_PI;
    this->thrust = this->myArmPropSimulator->GetPropThrust();

    // TEST 2020-12-27 ^^ augmenter vitesse affichage

    this->theta_dot = this->myArmPropSimulator->GetThetaDot();
    this->theta_dotdot = this->myArmPropSimulator->GetThetaDotDot();
    this->theta_dotdot_controlleur = this->myArmPropController->GetThetaDotdotCmd();

    this->ui->theta_value_label->setText(QString::number(this->degree_arm));
    this->ui->thrust_value_label->setText(QString::number(this->thrust));

    this->ui->graph_theta_value_label->setText(QString::number(this->degree_arm, 'f', 4));
    this->ui->graph_thrust_value_label->setText(QString::number(this->thrust, 'f', 4));
    this->ui->graph_theta_dot_value_label->setText(QString::number(this->theta_dot, 'f', 4));
    this->ui->graph_theta_dotdot_value_label->setText(QString::number(this->theta_dotdot, 'f', 4));
    this->ui->graph_theta_dotdot_controlleur_value_label->setText(QString::number(this->theta_dotdot_controlleur, 'f', 4));

    // T1 = Theta-Time
    if (this->ui->angle_unite_listBox->currentIndex() == 0) // "deg" -> unité: degré
    {
        this->graph_theta.dessiner_les_points(QPointF(this->timer_graph, this->degree_arm));
    }
    else // "rad" -> unité: radian
    {
        this->graph_theta.dessiner_les_points(QPointF(this->timer_graph, this->rad_arm));
    }

    // T2 = Thrust-Time
    this->graph_thrust.dessiner_les_points(QPointF(this->timer_graph, this->thrust));

    // T3 = Theta_Dot-Time
    this->graph_theta_dot.dessiner_les_points(QPointF(this->timer_graph, this->theta_dot));

    // T4 = Theta_DotDot-Time
    this->graph_theta_dotdot.dessiner_les_points(QPointF(this->timer_graph, this->theta_dotdot), QPointF(this->timer_graph, this->theta_dotdot_controlleur));

    // TEST 2020-12-27 ^^ augmenter vitesse affichage

    this->myDraw->definit_la_puissance_et_la_rotation_du_bras(this->thrust, this->degree_arm);
}
// ------------------------------------------------------
//                      WINDOW
// ------------------------------------------------------
// ------------------------------------------------------
//                      SERIAL PORT
// ------------------------------------------------------
void view_fly_arm::on_usb_port_radioButton_clicked()
{
    this->ui->com_port_comboBox->clear();
    this->connect_button_state();

    this->myHilsModeSerialCommunicator->myCommunicator->port_select(QSerialPortInfo::availablePorts()[0]);

    this->ui->terminal_info_label->setText("Press Connect ...");
}

void view_fly_arm::on_com_port_radioButton_clicked()
{
    this->find_ports();
    this->connect_button_state();

    this->myHilsModeSerialCommunicator->myCommunicator->port_select(QSerialPortInfo::availablePorts()[this->ui->com_port_comboBox->currentText().toInt()]);

    this->ui->terminal_info_label->setText("Press Connect ...");

}

void view_fly_arm::on_com_port_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    this->connect_button_state();
}

void view_fly_arm::on_baud_rate_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    this->connect_button_state();
}
// ------------------------------------------------------
//                      FORMULE
// ------------------------------------------------------
void view_fly_arm::slot_formule_a_tester()
{
//     qDebug() << "view_fly_arm::slot_formule_a_tester";

     this->buttons_enabled(true, false, false);

//    qDebug() << "\nla formule: " << this->myView_formule->formule();

    this->formule_variables_possibles_index = this->myModel_formule->donne_les_variables_possibles_utilisees_index();
    // qDebug() << "formule_variables_possibles_index: " << this->formule_variables_possibles_index;

    this->formule_valeurs.clear();

    // mettre les références des variables possibles dans formule_valeurs
    for(int index = 0; index < this->formule_variables_possibles_index.length(); index++)
    {
        if(this->formule_variables_possibles_index.at(index) == 0)
            this->formule_valeurs.append(this->myArmPropController->formule_GetThetaCmd());
        else
            this->formule_valeurs.append(this->myArmPropController->formule_GetThetaDotdotCmd());
    }
}

void view_fly_arm::view_formule_closed()
{
//    qDebug() << "view_fly_arm::view_formule_closed";

    this->buttons_enabled(false, false, false);
    this->formule_active = false;
    this->ui->mode_label->setText("");
}

void view_fly_arm::view_formule_modification()
{
//    qDebug() << "view_fly_arm::view_formule_modification";
    this->buttons_enabled(false, false, false);
}

void view_fly_arm::donne_a_la_formule_les_variables_possibles(void)
{
//     qDebug() << "view_fly_arm::donne_a_la_formule_les_variables_possibles";

    this->myModel_formule->recevoir_les_variables_possibles(this->formule_variables_possibles);
}

// ******************************************************
//                      START LOCAL FUNCTIONS
// ******************************************************
// ------------------------------------------------------
//                      INIT
// ------------------------------------------------------
void view_fly_arm::objects_init(void)
{
    this->myArmPropSimulator = ArmPropSimulator::getInstance();
    this->myArmPropController = ArmPropController::getInstance();
    this->myHilsModeSerialCommunicator = hilsModeSerialCommunicator::getInstance();
    this->myThreadSimulatorController = threadSimulatorController::getInstance();
    this->myThreadSimulatorController_is_create = false;

    this->myModel_file_setting = model_file_setting::getInstance();
    this->myTime = model_time::getInstance();
    this->myModel_setting_PC_controller = model_setting_PC_controller::getInstance();
    this->my_model_setting_sample_time = model_setting_sample_time::getInstance();

    // FORMULE
    this->myModel_formule = model_formule::getInstance();
    this->myView_formule = new view_formule();

    // TIMER
    this->timer1 = new QBasicTimer();
//    this->timer1 = new QTimer(this);
//    connect(this->timer1, SIGNAL(timeout()), this, SLOT(timer1_Tick()));
}

void view_fly_arm::sample_time_init(void)
{
    this->step = this->my_model_setting_sample_time->step_get();
//    qDebug() << "this->step" << this->step;
    this->simulator_step = this->my_model_setting_sample_time->simulator_step_get();
//    qDebug() << "this->simulator_step" << this->simulator_step;
    this->controller_step = this->my_model_setting_sample_time->controller_step_get();
//    qDebug() << "this->controller_step" << this->controller_step;
    this->graph_step = this->my_model_setting_sample_time->graph_step_get();
//    qDebug() << "this->graph_step" << this->graph_step;
    this->simulator_step_count = 0;
    this->controller_step_count = 0;
    this->graph_step_count = 0;
}

void view_fly_arm::graph_init(void)
{
    // DESSIN
    this->myDraw = new model_draw(this->ui->view_panel);
    this->myDraw->definit_la_taille(this->ui->view_panel->size());
//    qDebug() << "view_panel->rect = " << this->ui->view_panel->rect();

    // GRAPHIQUES
    this->graph_theta_dotdot.ajoute_une_deuxieme_ligne();

    this->ui->splitter_2->addWidget(this->graph_theta.donne_le_qchartview());
    this->ui->splitter_2->addWidget(this->graph_thrust.donne_le_qchartview());
    this->ui->splitter_2->addWidget(this->graph_theta_dot.donne_le_qchartview());
    this->ui->splitter_2->addWidget(this->graph_theta_dotdot.donne_le_qchartview());

//    this->remplir_valeurs();
    this->graph_after_change_sample_time();
}

void view_fly_arm::graph_after_change_sample_time(void)
{
    this->timer_graph_step = double(this->step * this->graph_step / 1000.0);
    this->timer_graph = 0;
    this->graphs_draw();
}
void view_fly_arm::attributs_init(void)
{
    // TIMER
    this->tick_compteur = 0;
    this->simulator_step_count = 0;
    this->controller_step_count = 0;
    this->graph_step_count = 0;
    this->timer_graph = 0;

    // AFFICHAGE
    this->bouton_racine = "border-image: url(:/images/";
    this->bouton_gris = "_gris.jpg);";
    this->bouton_vert = "_green.jpg);";

    this->hils_mode_choisi = false;

    this->rad_arm = 0.0;
    this->degree_arm = 0;
    this->thrust = 0.0;
    this->theta_dot = 0.0;
    this->theta_dotdot = 0.0;
    this->theta_dotdot_controlleur = 0.0;

    this->time_desired = this->myTime->time_desired_get();
    this->ui->time_textBox->setText(QString::number(this->time_desired));
    this->tick_max = this->time_desired * 1000 / this->step;

    this->DesiredTheta_Rad = 0.0;
    this->DesiredTheta_Deg = 0.0;

    this->consol_is_show = false;
    this->theta_desired_is_show =false;

    this->ui->angle_unite_listBox->addItem("Deg");
    this->ui->angle_unite_listBox->addItem("Rad");
    this->ui->angle_unite_listBox->setCurrentIndex(0);

    // FORMULE
    this->formule_active = false;
    this->formule_variables_possibles << "Theta" << "ThetaDotDot";
    this->myModel_formule->recevoir_les_variables_possibles(this->formule_variables_possibles);
}

void view_fly_arm::widgets_hide(void)
{
//    this->ui->theta_OR_thrust_desired_label->setVisible(false);
//    this->ui->theta_OR_thrust_desired_value_label->setVisible(false);
//    this->ui->theta_desired_unite_label->setVisible(false);
    this->ui->modes_panel->setFixedSize(this->ui->modes_panel->width(), 128);

    this->theta_desired_show_OR_hide(false);
    this->consol_show_OR_hide(false);
}

void view_fly_arm::update_graphs_sample_time_timer(void)
{
    // mise-à-jour des informations
//    qDebug() << "mettre a jour les informations apres modification de Sample Time";
    this->graphs_clear();
    this->sample_time_init();
    this->graph_after_change_sample_time();
}

void view_fly_arm::update_controller_type()
{
//    qDebug() << "2 - update_controller_type";

    if (this->myModel_setting_PC_controller->controller_type_get() == CASCADE_CONTROLLER)
    {
//        qDebug() << "update_controller_type --> CASCADE_CONTROLLER";
        this->controller_type_name = "(Cascade)";

//        qDebug() << "K1 = " << QString::number(this->myModel_setting_PC_controller->k1_get());
//        qDebug() << "K2 = " << QString::number(this->myModel_setting_PC_controller->k2_get());
    }
    else // LEADLAG_CONTROLLER
    {
//        qDebug() << "update_controller_type --> LEADLAG_CONTROLLER";
        this->controller_type_name = "(Leadlag)";

//        qDebug() << "A1 = " << QString::number(this->myModel_setting_PC_controller->a1_get());
//        qDebug() << "A2 = " << QString::number(this->myModel_setting_PC_controller->a2_get());
//        qDebug() << "B1 = " << QString::number(this->myModel_setting_PC_controller->b1_get());
//        qDebug() << "B2 = " << QString::number(this->myModel_setting_PC_controller->b2_get());
//        qDebug() << "B3 = " << QString::number(this->myModel_setting_PC_controller->b3_get());
    }
    //this->modes_infos_communes(sender, e, true, false, false, true, L"Mode: PC control " + this->controller_type_name, HILS_MODE_PC_CONTROLLER);
    if(this->theta_desired == true)
    {
        this->modes_infos_communes(true, false, false, true, "Mode: PC control " + this->controller_type_name, HILS_MODE_PC_CONTROLLER);
    }
}
// ------------------------------------------------------
//                      MODES
// ------------------------------------------------------
void view_fly_arm::modes_infos_communes(bool consol_is_show_bool, bool consol_show_OR_hide_bool, bool theta_OR_thrust_desired_is_show_bool, bool theta_OR_thrust_desired_show_OR_hide_bool, QString texte, int hils_mode_int)
{
    this->disconnect_serial_port();
    this->myArmPropSimulator->init();
    this->myArmPropController->init();

    // mettre à jour ThetaCmd ou ThrustCmd
    if(this->ui->theta_OR_thrust_desired_trackBar->isVisible())
        this->theta_OR_thrust_desired_save(this->ui->theta_OR_thrust_desired_trackBar->value());

//    this->myOpenGL->render();
    this->myHilsModeSerialCommunicator->setHilsMode(hils_mode_int);
    this->myThreadSimulatorController->hils_mode_update(hils_mode_int);
    // mettre les boutons 'PLAY' 'PAUSE' et 'STOP' avec les valeurs de départ
    this->buttons_enabled(true, false, false);
    // adapter la taille de 'modes_panel' en fonction de hils_mode
    if(this->myHilsModeSerialCommunicator->getHilsMode() == HILS_MODE_PC_CONTROLLER || this->myHilsModeSerialCommunicator->getHilsMode() == HILS_MODE_MANUAL_THRUST_COMMAND)
    {
        this->ui->modes_panel->setFixedSize(this->ui->modes_panel->width(), 180);
        // mettre les controles devant 'modes_panel'
        this->ui->theta_OR_thrust_desired_label->setVisible(true);
        this->ui->theta_OR_thrust_desired_value_label->setVisible(true);
        this->ui->theta_desired_unite_label->setVisible(true);
    }
    else
    {
        this->ui->theta_OR_thrust_desired_label->setVisible(false);
        this->ui->theta_OR_thrust_desired_value_label->setVisible(false);
        this->ui->theta_desired_unite_label->setVisible(false);
        this->ui->modes_panel->setFixedSize(this->ui->modes_panel->width(), 128);
    }
    // afficher les informations nécessaires
    if (this->consol_is_show == consol_is_show_bool)
    {
        this->consol_show_OR_hide(consol_show_OR_hide_bool);
    }

    if (theta_desired_is_show == theta_OR_thrust_desired_is_show_bool)
    {
        this->theta_desired_show_OR_hide(theta_OR_thrust_desired_show_OR_hide_bool);
    }

    this->graphs_clear();
    this->ui->mode_label->setText(texte);
    this->timer_graph = 0;
    this->graphs_draw();
    this->simulator_step_count = 0;
    this->controller_step_count = 0;
    this->graph_step_count = 0;
    this->ui->graph_thrust_value_label->setText("0.0");
    this->ui->graph_theta_value_label->setText("0.0");
    this->ui->theta_value_label->setText("0.0");
    this->ui->thrust_value_label->setText("0.0");
}
// ------------------------------------------------------
//                      PANEL MODE
// ------------------------------------------------------
void view_fly_arm::buttons_enabled(bool play_state,bool pause_state, bool stop_state)
{
    this->ui->play_button->setEnabled(play_state);
    this->ui->pause_button->setEnabled(pause_state);
    this->ui->stop_button->setEnabled(stop_state);

    // CHANGER LES COULEURS DES BOUTONS
    if(play_state)
        this->ui->play_button->setStyleSheet(this->bouton_racine + "play" + this->bouton_vert);
    else
        this->ui->play_button->setStyleSheet(this->bouton_racine + "play" + this->bouton_gris);

    if(pause_state)
        this->ui->pause_button->setStyleSheet(this->bouton_racine + "pause" + this->bouton_vert);
    else
        this->ui->pause_button->setStyleSheet(this->bouton_racine + "pause" + this->bouton_gris);

    if(stop_state)
        this->ui->stop_button->setStyleSheet(this->bouton_racine + "stop" + this->bouton_vert);
    else
        this->ui->stop_button->setStyleSheet(this->bouton_racine + "stop" + this->bouton_gris);
}

void view_fly_arm::theta_desired_show_OR_hide(bool true_or_false)
{
    this->theta_desired_is_show = true_or_false;

    this->ui->theta_OR_thrust_desired_label->setVisible(true_or_false);
    this->ui->theta_OR_thrust_desired_value_label->setVisible(true_or_false);
    this->ui->theta_OR_thrust_desired_trackBar->setVisible(true_or_false);
    this->ui->theta_desired_unite_label->setVisible(true_or_false);
}

void view_fly_arm::theta_OR_thrust_desired_save(int new_value)
{
    // theta_desired
    if (this->theta_desired == true)
    {
//        qDebug() << "desired_value_label = " << this->ui->theta_OR_thrust_desired_value_label->text();
//        qDebug() << "desired_trackBar = " << QString::number(action);
//        qDebug() << "desired_trackBar = " << QString::number(this->ui->theta_OR_thrust_desired_trackBar->value());

//        this->ui->theta_OR_thrust_desired_value_label->setText(QString::number(this->ui->theta_OR_thrust_desired_trackBar->value()));
//        this->DesiredTheta_Rad = double(this->ui->theta_OR_thrust_desired_trackBar->value()) * MY_PI / 180.0;
//        qDebug() << "trackbar-theta_desired = " << QString::number(new_value);
        this->ui->theta_OR_thrust_desired_value_label->setText(QString::number(new_value));
        this->DesiredTheta_Rad = double(new_value) * MY_PI / 180.0;

//        this->ui->theta_OR_thrust_desired_value_label->setText(QString::number(action));
//        this->DesiredTheta_Rad = double(action) * MY_PI / 180.0;
        this->myArmPropController->SetThetaCmd(this->DesiredTheta_Rad);
    }
    // thrust_desired
    else
    {
//        this->thrust_desired = this->ui->theta_OR_thrust_desired_trackBar->value() / 10.0;
//        this->thrust_desired = action / 10.0;
//        this->ui->theta_OR_thrust_desired_value_label->setText(QString::number(this->thrust_desired));
//        this->myArmPropController->SetThrustCmd(this->thrust_desired);
        this->thrust_desired = double(new_value) / 10.0;
//        this->thrust_desired = action / 10.0;
        this->ui->theta_OR_thrust_desired_value_label->setText(QString::number(this->thrust_desired));
        this->myArmPropController->SetThrustCmd(this->thrust_desired);
    }
}
// ------------------------------------------------------
//                      CONSOLE
// ------------------------------------------------------
void view_fly_arm::consol_show_OR_hide(bool true_or_false)
{
    this->consol_is_show = true_or_false;

    this->ui->terminal_panel->setVisible(true_or_false);

    if(true_or_false == true)
        this->ui->baud_rate_comboBox->setCurrentText("19200");
}

void view_fly_arm::connect_disconnect_true_or_false(bool true_or_false)
{
    this->ui->connect_button->setEnabled(true_or_false);
    this->ui->disconnect_button->setEnabled(!true_or_false);
    this->ui->usb_port_radioButton->setEnabled(true_or_false);
    this->ui->com_port_radioButton->setEnabled(true_or_false);
    this->ui->com_port_comboBox->setEnabled(true_or_false);
    this->ui->baud_rate_comboBox->setEnabled(true_or_false);

    if(true_or_false == false)
    {
        this->ui->status_panel->setStyleSheet("background-color: rgb(0, 255, 0);"); // Qt::green
        this->ui->status_label->setText("Connected");
    }
    else
    {
        this->ui->status_panel->setStyleSheet("background-color: rgb(255, 0, 0);"); // Qt::red
        this->ui->status_label->setText("Disconnect");
    }
}
// ------------------------------------------------------
//                      GRAPHS
// ------------------------------------------------------
void view_fly_arm::graphs_clear(void)
{
    this->graph_theta.clear();
    this->graph_thrust.clear();
    this->graph_theta_dot.clear();
    this->graph_theta_dotdot.clear();
}
// ------------------------------------------------------
//                      SERIAL PORT
// ------------------------------------------------------
void view_fly_arm::find_ports(void)
{
    // vider la liste
    this->ui->com_port_comboBox->clear();
    // obtient le nom des ports utilisant la liaison série
    QList<QSerialPortInfo> liste_de_ports = QSerialPortInfo::availablePorts();
    // ajoute à la combobox les ports trouvés
    for(int index = 0; index < liste_de_ports.count(); index++)
    {
        this->ui->com_port_comboBox->addItem(liste_de_ports.at(index).portName());
    }
}

void view_fly_arm::connect_button_state(void)
{
    // enabled SI
    // com port actif ET baud rate valeur sélectionnée ET com port combobox choisi
    // OU usb port actif ET baud rate valeur sélectionnée
    if(this->ui->baud_rate_comboBox->currentIndex() >= 0 && ((this->ui->com_port_radioButton->isChecked() && this->ui->com_port_comboBox->currentIndex() >= 0) || this->ui->usb_port_radioButton->isChecked()))
        this->ui->connect_button->setEnabled(true);
    else
        this->ui->connect_button->setEnabled(false);
}

void view_fly_arm::disconnect_serial_port(void)
{
    // déconnecter la liaison série lorsque l'utilisateur change de HilsMode sans avoir déconnecter manuellement
    if(this->ui->disconnect_button->isEnabled())
        this->disconnect_method();
}

void view_fly_arm::timer_ajuster_init()
{
    // ajuster le temps d'exécution de QBasicTimer
    this->temps_execution_theorique = 0;
    this->temps_execution_reel = 0;
    this->temps_a_rattraper = 0;
}

void view_fly_arm::variables_valeurs_a_enregistrer_initialisation()
{
    QString hils_mode;
    int hils_mode_index;
    QString type_controller;

    type_controller = "";

    hils_mode_index = this->myHilsModeSerialCommunicator->getHilsMode();
    if(hils_mode_index == 0)
    {
        hils_mode = "HILS_MODE_PC_CONTROLLER";

        if(this->myModel_setting_PC_controller->controller_type_get() == CASCADE_CONTROLLER)
            type_controller = "CASCADE_CONTROLLER";
        else
            type_controller = "LEADLAG_CONTROLLER";
    }
    else if(hils_mode_index == 1)
        hils_mode = "HILS_MODE_1_";
    else if(hils_mode_index == 2)
        hils_mode = "HILS_MODE_REAL_ANGLE";
    else if(hils_mode_index == 3)
        hils_mode = "HILS_MODE_3_";
    else if(hils_mode_index == 4)
        hils_mode = "HILS_MODE_MANUAL_THRUST_COMMAND";

    this->variables_valeurs_enregistrees.clear();
    this->variables_valeurs_enregistrees.append("Classe: view_fly_arm");
    this->variables_valeurs_enregistrees.append('\n');
    this->variables_valeurs_enregistrees.append("Hils_mode:" + hils_mode + '\n');
    if(hils_mode_index == 0)
        this->variables_valeurs_enregistrees.append("type_controller:" + type_controller + '\n');
    this->variables_valeurs_enregistrees.append("tick_max:" + QString::number(this->tick_max) + '\n');
    this->variables_valeurs_enregistrees.append("step:" + QString::number(this->step) + '\n');
    this->variables_valeurs_enregistrees.append("controller_step:" + QString::number(this->controller_step) + '\n');
    this->variables_valeurs_enregistrees.append("simulator_step:" + QString::number(this->simulator_step) + '\n');
    this->variables_valeurs_enregistrees.append("graph_step:" + QString::number(this->graph_step) + '\n');
    this->variables_valeurs_enregistrees.append("time_desired:" + QString::number(this->time_desired) + '\n');
    this->variables_valeurs_enregistrees.append('\n');
    this->variables_valeurs_enregistrees.append("Classe: view_fly_arm::::::::Classe: ArmPropController::Classe: ArmPropSimulator");
    this->variables_valeurs_enregistrees.append('\n');
    this->variables_valeurs_enregistrees.append("tick_compteur:DesiredTheta_Rad:DesiredTheta_Deg:signalValue:thrust_desired:degree_arm:theta_dotdot_controlleur:timer_graph:timer_graph_step:thetaCmd_:theta_dotdot_cmd:propThrustcmd:theta_:theta_dot_:theta_dotdot_:prop_thrust_");
    this->variables_valeurs_enregistrees.append('\n');
}

void view_fly_arm::variables_valeurs_a_enregistrer(const int partie)
{
    /*
     * VALEURS EN DEBUT DE TEXTE POUR LE CONTEXTE
     *    Classe: view_fly_arm
     *        variables
     *            - tick_max
     *            - step
     *            - simulator_step
     *            - controller_step
     *            - graph_step
     *            - time_desired
     *
     * VALEURS POUR CHAQUE TICK DU TIMER
     *    Classe: view_fly_arm
     *        variables
     *            - tick_compteur
     *            - DesiredTheta_Rad
     *            - DesiredTheta_Deg
     *            - signalValue
     *            - thrust_desired
     *            - rad_arm
     *            - degree_arm
     *            - thrust
     *            - theta_dot
     *            - theta_dotdot
     *            - theta_dotdot_controlleur
     *            - timer_graph
     *            - timer_graph_step
     *
     *    Classe: ArmPropController
     *        variables
     *            - thetaCmd_
     *            - theta_dot_cmd ? AUCUNE COMMANDE POUR RECUPERER CETTE VARIABLE
     *            - theta_dotdot_cmd
     *            - propThrustcmd
     *
     *    Classe: ArmPropSimulator
     *        variables
     *            - theta_
     *            - theta_dot_
     *            - theta_dotdot_
     *            - prop_thrust_
     *
     */

    if(partie == DONNEES_DEBUT)
    {
        this->variables_valeurs_timer.clear();
        this->variables_valeurs_ArmPropController.clear();
        this->variables_valeurs_ArmPropSimulator.clear();
        this->variables_valeurs_graph_1.clear();
        this->variables_valeurs_graph_2.clear();
    }
    else if(partie == DONNEES_TIMER)
        this->variables_valeurs_timer.append(QString::number(this->tick_compteur,'f', 4) + ":" + QString::number(this->DesiredTheta_Rad,'f', 4) + ":" + QString::number(this->DesiredTheta_Deg,'f', 4) + ":" + QString::number(this->signalValue,'f', 4) + ":" + QString::number(this->thrust_desired,'f', 4) + ":");
    else if(partie == DONNEES_ARMPROPCONTROLLER)
        this->variables_valeurs_ArmPropController.append(QString::number(this->myArmPropController->GetThetaCmd(),'f', 4) + ":" + QString::number(this->myArmPropController->GetThetaDotdotCmd(),'f', 4) + ":" + QString::number(this->myArmPropController->GetThrustCmd(),'f', 4) + ":");
    else if(partie == DONNEES_ARMPROPSIMULATOR)
        this->variables_valeurs_ArmPropSimulator.append(QString::number(this->myArmPropSimulator->GetTheta(),'f', 4) + ":" + QString::number(this->myArmPropSimulator->GetThetaDot(),'f', 4) + ":" + QString::number(this->myArmPropSimulator->GetThetaDotDot(),'f', 4) + ":" + QString::number(this->myArmPropSimulator->GetPropThrust(),'f', 4));
    else if(partie == DONNEES_GRAPH_1)
        this->variables_valeurs_graph_1.append(QString::number(this->timer_graph,'f', 4) + ":" + QString::number(this->timer_graph_step,'f', 4) + ":");
    else if(partie == DONNEES_GRAPH_2)
        this->variables_valeurs_graph_2.append(QString::number(this->degree_arm,'f', 4) + ":" + QString::number(this->theta_dotdot_controlleur,'f', 4) + ":");
    else if(partie == DONNEES_FIN)
    {
        if(this->variables_valeurs_ArmPropController.isEmpty())
            this->variables_valeurs_ArmPropController.append(":::");
        if(this->variables_valeurs_ArmPropSimulator.isEmpty())
            this->variables_valeurs_ArmPropSimulator.append(":::");
        if(this->variables_valeurs_graph_2.isEmpty())
            this->variables_valeurs_graph_2.append("::");

        this->variables_valeurs_enregistrees.append(this->variables_valeurs_timer + this->variables_valeurs_graph_2 + this->variables_valeurs_graph_1 + this->variables_valeurs_ArmPropController + this->variables_valeurs_ArmPropSimulator + '\n');
    }
}














// ==========================================================
// ==========================================================
// ==========================================================
// ==========================================================


// FONCTIONS TEMPORAIRES
void view_fly_arm::remplir_valeurs()
{
    for(qreal index = 0.1; index < 15; index += 0.1)
    {
        this->valeurs_temp = QPointF(qreal(index), qreal(this->generateur_nombre_aleatoire.bounded(-23, 42)));
        this->graph_theta.dessiner_les_points(this->valeurs_temp);
        this->graph_thrust.dessiner_les_points(this->valeurs_temp);
        this->valeur2 = this->valeurs_temp;
        this->valeur2.setY(this->valeurs_temp.y() + 4);
        this->graph_theta_dot.dessiner_les_points(this->valeurs_temp, this->valeur2);
//        this->graph_theta_dot.dessiner_les_points(this->valeurs_temp);
        this->graph_theta_dotdot.dessiner_les_points(this->valeurs_temp);
    }
    // valeurs_temp DIVERSES
//    this->valeurs_temp = {{0.1, 0}, {0.2, 3}, {0.3, 6}, {0.4, -2}, {0.5, -7},
//                    {0.6, 10.8}, {0.7, 23}, {0.8, 16}, {0.9, -2.7}, {1, -7.5},
//                    {1.1, -0.3}, {1.2, 43}, {1.3, 1.6}, {1.4, 25}, {1.5, -0.7},
//                    {1.6, -9.9}, {1.7, -10}, {1.8, 16.8}, {1.9, -2.1}, {2, -5.5}};
    // TEST valeurs_temp Y > 0 ET Y < 0.1
//    this->valeurs_temp = {{0.1, 0}, {0.2, 0.03}, {0.3, 0.04}, {0.4, 0.01}, {0.5, 0.03},
//                    {0.6, 0.099}, {0.7, 0.07}, {0.8, 0.08548}};
}
