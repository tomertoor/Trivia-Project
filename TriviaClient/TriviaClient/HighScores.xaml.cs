using System;
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
    public partial class HighScores : Window
    {
        private static User loggedUser;
        public HighScores()
        {
            InitializeComponent();
            loggedUser = Stats.loggedUser;
            //request the high scores
            try
            {
                string data = Consts.HIGH_SCORES + "0000";
                loggedUser.SendData(data, loggedUser.sock);
                ServerMsg msg = loggedUser.GetData();
                HighscoresResponse res = new HighscoresResponse();
                switch (msg.code)
                {
                    case Consts.HIGH_SCORES:
                        msg.data = msg.data.Remove(0, 1);
                        msg.data = msg.data.Remove(msg.data.Length - 1, 1);
                        res = JsonSerializer.Deserialize<HighscoresResponse>(msg.data);
                        break;
                    case Consts.ERROR:
                        this.message.Text = msg.data;
                        break;
                }
                if (res.status.Equals("1"))
                {
                    this.user1.Text = res.users[0];
                    this.user2.Text = res.users[1];
                    this.user3.Text = res.users[2];
                }
                else
                {
                    ErrorResponse errorResponse = new ErrorResponse();
                    msg.data = msg.data.Remove(0, 1);
                    errorResponse = JsonSerializer.Deserialize<ErrorResponse>(msg.data);
                    this.message.Text = errorResponse.message;
                }
            }
            catch (Exception)
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
            Menu menu = new Menu();
            this.Close();
            menu.Show();
        }
    }

    public class HighscoresResponse
    {
        public string status { get; set; }
        public string[] users { get; set; }
        public HighscoresResponse()
        {
            status = "";
            users = new string[5];
        }
    }
}
