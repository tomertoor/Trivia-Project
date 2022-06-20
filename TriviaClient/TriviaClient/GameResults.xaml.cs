using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Text.Json;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for GameResults.xaml
    /// </summary>
    public partial class GameResults : Window
    {
        public static User loggedUser;
        public GameResults(Window w)
        {
            InitializeComponent();
            loggedUser = Game.loggedUser;
            AppDomain.CurrentDomain.ProcessExit += (sender, eventArgs) =>
            {
                loggedUser.Logout();
            };
            try
            {
                string data = Consts.GET_RESULTS.PadLeft(2, '0') + "0000";
                loggedUser.SendData(data, loggedUser.sock);
                ServerMsg msg = loggedUser.GetData();
                GetGameResults res = new GetGameResults();
                switch (msg.code)
                {
                    case Consts.GET_RESULTS:
                        res = JsonSerializer.Deserialize<GetGameResults>(msg.data);
                        break;
                    case Consts.ERROR:
                        message.Text = msg.data;
                        break;
                }
                if (res.status == Consts.OK_STATUS)
                {
                    ShowScores(res.results);
                }
                else
                {
                    ErrorResponse errorResponse = new ErrorResponse();
                    errorResponse = JsonSerializer.Deserialize<ErrorResponse>(msg.data);
                    message.Text = errorResponse.message;
                }
            }
            catch (Exception)
            {
                message.FontSize = 25;
                message.Text = "Error occured";
            }
        }

        private void ShowScores(List<GameResult> users)
        {
            scores.Children.Clear();

            foreach (GameResult user in users)
            {
                Label newLabel = new Label();
                newLabel.Content = "Username: " + user.username + ",Correct answers count: " + user.correctAnswerCount.ToString() + ",Wrong answers count: " + user.wrongAnswerCount.ToString() + ",Average time answer: " + user.averageAnswerTime.ToString();
                newLabel.FontSize = 13;
                this.scores.Children.Add(newLabel);
            }
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        struct GameResult
        {
            public string username { get; set; }
            public int correctAnswerCount { get; set; }
            public int wrongAnswerCount { get; set; }
            public float averageAnswerTime { get; set; }

        
        }

        class GetGameResults
        {
            public int status { get; set; }
            public List<GameResult> results { get; set; }

            public GetGameResults()
            {
                status = 0;
                results = new List<GameResult>();
            }
        }

        private void menu_Click(object sender, RoutedEventArgs e)
        {
            loggedUser.passedWhat = Consts.GET_RESULTS;
            Menu menu = new Menu(this);
            this.Close();
            menu.Show();
        }
    }
}
