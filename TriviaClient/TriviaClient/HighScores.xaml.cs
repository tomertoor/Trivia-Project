using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Input;
using System.Text.Json;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for UserStats.xaml
    /// </summary>
    public partial class HighScores : Window
    {
        public static User loggedUser;
        //constructor
        public HighScores(Window w)
        {
            InitializeComponent();
            this.Left = w.Left;
            this.Top = w.Top;
            loggedUser = Stats.loggedUser;
            AppDomain.CurrentDomain.ProcessExit += (sender, eventArgs) =>
            {
                loggedUser.Logout();
            };
            //request the high scores
            try
            {
                string data = Consts.HIGH_SCORES.PadLeft(2, '0') + "0000";
                loggedUser.SendData(data, loggedUser.sock);
                ServerMsg msg = loggedUser.GetData();
                HighscoresResponse res = new HighscoresResponse();
                switch (msg.code)
                {
                    case Consts.HIGH_SCORES:
                        res = JsonSerializer.Deserialize<HighscoresResponse>(msg.data);
                        break;
                    case Consts.ERROR:
                        this.message.Text = msg.data;
                        break;
                }
                if (res.status == Consts.OK_STATUS)
                {
                    //in case of success
                    if(res.topScores.Count >= 3)
                    {
                        this.user1.Text += res.topScores[0];
                        this.user2.Text += res.topScores[1];
                        this.user3.Text += res.topScores[2];
                    }
                    else if (res.topScores.Count == 2)
                    {
                        this.user1.Text += res.topScores[0];
                        this.user2.Text += res.topScores[1];
                    }
                    else if (res.topScores.Count == 1)
                    {
                        this.user1.Text += res.topScores[0];
                    }
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

        //to enable moving the window throug the screen
        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        //function to handle the return to menu button click
        private void menu_Click(object sender, RoutedEventArgs e)
        {
            loggedUser.passedWhat = Consts.HIGH_SCORES;
            Menu menu = new Menu(this);
            this.Close();
            menu.Show();
        }
    }

    //class for high scores response deserializing
    public class HighscoresResponse
    {
        public int status { get; set; }
        public List<string> topScores { get; set; }
        public HighscoresResponse()
        {
            status = 0;
        }
    }
}
