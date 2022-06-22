using System;
using System.Windows;
using System.Windows.Input;
using System.Text.Json;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for UserStats.xaml
    /// </summary>
    public partial class UserStats : Window
    {
        public static User loggedUser;
        //construntor
        public UserStats()
        {
            InitializeComponent();
            AppDomain.CurrentDomain.ProcessExit += (sender, eventArgs) =>
            {
                loggedUser.Logout();
            };
            loggedUser = Stats.loggedUser;
            message.Text = loggedUser.username + " statistics";
            //show the user stats
            try
            {
                string data = Consts.PERSONAL_STATS.PadLeft(2, '0') + "0000";
                loggedUser.SendData(data, loggedUser.sock);
                ServerMsg msg = loggedUser.GetData();
                MystatsResponse res = new MystatsResponse();
                switch (msg.code)
                {
                    case Consts.PERSONAL_STATS:
                        res = JsonSerializer.Deserialize<MystatsResponse>(msg.data);
                        break;
                    case Consts.ERROR:
                        this.message.Text = msg.data;
                        break;
                }
                if(res.status == Consts.OK_STATUS)
                {
                    //in case of success
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
        
        //to enable moving the window throug the screen
        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        //function for handling return to menu button click
        private void menu_Click(object sender, RoutedEventArgs e)
        {
            loggedUser.passedWhat = Consts.PERSONAL_STATS;
            Menu menu = new Menu(this);
            this.Close();
            menu.Show();
        }
    }

    //class for personal stats response deserializing
    public class MystatsResponse
    {
        public int status { get; set; }
        public string gameCount { get; set; }
        public string totalAnswers { get; set; }
        public string correctAnswers { get; set; }
        public string averageAnswerTime { get; set; }

        public MystatsResponse()
        {
            status = 0;
            gameCount = "";
            totalAnswers = "";
            correctAnswers = "";
            averageAnswerTime = "";
        }
    }
}
