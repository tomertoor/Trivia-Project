using System;
using System.Collections.Generic;
using System.Text;
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
                    ShowScores(res.users);
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

        private void ShowScores(List<List<JsonElement>> users)
        {
            //username, correct, wrong, average
            scores.Children.Clear();

            foreach (List<JsonElement> user in users)
            {
                Label newLabel = new Label();
                newLabel.Content = "Username: " + user[0] + ",Correct answers count: " + user[1].ToString() + ",Wrong answers count: " + user[2].ToString() + ",Average time answer: " + user[3].ToString();
                newLabel.FontSize = 10;
                this.scores.Children.Add(newLabel);
            }
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        class GetGameResults
        {
            public int status { get; set; }
            public List<List<JsonElement>> users { get; set; }

            public GetGameResults()
            {
                status = 0;
                users = new List<List<JsonElement>>();
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
