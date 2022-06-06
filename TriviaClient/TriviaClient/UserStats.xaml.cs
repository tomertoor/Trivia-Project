﻿using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Text.Json;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for UserStats.xaml
    /// </summary>
    public partial class UserStats : Window
    {
        public static User loggedUser;
        public UserStats()
        {
            InitializeComponent();
            loggedUser = Stats.loggedUser;
            message.Text = loggedUser.username + " statistics";
            //show the user stats
            try
            {
                string data = Consts.PERSONAL_STATS + "0000";
                loggedUser.SendData(data, loggedUser.sock);
                ServerMsg msg = loggedUser.GetData();
                MystatsResponse res = new MystatsResponse();
                switch (msg.code)
                {
                    case Consts.PERSONAL_STATS:
                        /*msg.data = msg.data.Remove(0, 1);
                        msg.data = msg.data.Remove(msg.data.Length - 1, 1);*/
                        res = JsonSerializer.Deserialize<MystatsResponse>(msg.data);
                        break;
                    case Consts.ERROR:
                        this.message.Text = msg.data;
                        break;
                }
                if(res.status == Consts.OK_STATUS)
                {
                    this.correctAnswers.Text += " " + res.correctAnswers;
                    this.avgTime.Text += " " + res.averageAnswerTime.Substring(0, res.averageAnswerTime.Length-4); //only the first 4 digits 
                    this.noOfGames.Text += " " + res.gameCount;
                    this.totalAnswers.Text += " " + res.totalAnswers;
                }
                else
                {
                    ErrorResponse errorResponse = new ErrorResponse();
                    msg.data = msg.data.Remove(0, 1);
                    errorResponse = JsonSerializer.Deserialize<ErrorResponse>(msg.data);
                    this.message.Text = errorResponse.message;
                }
            }
            catch(Exception)
            {
                this.message.FontSize = 25;
                this.message.Text = "Error occured";
            }
        }
        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        private void menu_Click(object sender, RoutedEventArgs e)
        {
            loggedUser.passedWhat = Consts.PERSONAL_STATS;
            Menu menu = new Menu();
            this.Close();
            menu.Show();
        }
    }

    public class MystatsResponse
    {
        public int status { get; set; }
        public string gameCount { get; set; }
        public string totalAnswers { get; set; }
        public string correctAnswers { get; set; }
        public string averageAnswerTime { get; set; }
    }
}
