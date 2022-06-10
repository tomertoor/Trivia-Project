using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;
using System.Text.Json;
using System.Threading;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for Game.xaml
    /// </summary>
    public partial class Game : Window
    {
        private static User loggedUser;
        private static int timeForQ;
        private static DispatcherTimer Timer;
        private static int time;
        private static int quesCount;
        private static GetQuestionRes currectQuestion;
        public Game()
        {
            InitializeComponent();
            loggedUser = Room.loggedUser;
            timeForQ = Room.qTimeout;
            quesCount = Room.quesCount;
            StartGame();
        }

        private void StartGame()
        {
            for(int i = 1; i<=quesCount; i++)
            {
                GetQuestion();
                this.quesNum.Text = this.quesNum.Text.Substring(0, this.quesNum.Text.LastIndexOf(' ') + 1) + i.ToString();
                WaitForNextQuestion(5);//wait 5 seconds between each question
            }
        }

        private void WaitForNextQuestion(int seconds)
        {
            Thread.Sleep(seconds * 1000);
        }

        private void GetQuestion()
        {
            try
            {
                string data = Consts.GET_QUESTION.PadLeft(2, '0') + "0000";
                loggedUser.SendData(data, loggedUser.sock);
                ServerMsg msg = loggedUser.GetData();
                GetQuestionRes res = new GetQuestionRes();
                switch (msg.code)
                {
                    case Consts.GET_QUESTION:
                        res = JsonSerializer.Deserialize<GetQuestionRes>(msg.data);
                        break;
                    case Consts.ERROR:
                        message.Text = msg.data;
                        break;
                }
                if (res.status == Consts.OK_STATUS)
                {
                    currectQuestion = res;
                    AddQuestion();
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
        private void AddQuestion()
        {
            this.question.Text = currectQuestion.question;
            this.ans1.Content = currectQuestion.answers[0];
            this.ans2.Content = currectQuestion.answers[1];
            this.ans3.Content = currectQuestion.answers[2];
            this.ans4.Content = currectQuestion.answers[3];
            SetTimer();
        }

        private void SetTimer()
        {
            time = timeForQ;
            Timer = new DispatcherTimer();
            Timer.Interval = new TimeSpan(0, 0, 1);
            Timer.Tick += Timer_Tick;
            Timer.Start();
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            if(time > 0)
            {
                time--;
                countdown.Text = string.Format("00:0{0}:{1}", time / 60, time % 60);
            }
            else
            {
                countdown.Text = "Time over!";
                Timer.Stop();
            }
        }

        class GetQuestionRes
        {
            public int status { get; set; }
            public string question { get; set; }
            public List<string> answers { get; set; }
            public int correctAns { get; set; }
            public GetQuestionRes()
            {
                status = 0;
                question = "";
                answers = new List<string>();
            }
        }

        private void ans_Click(object sender, RoutedEventArgs e)
        {
            Button btn = (Button)sender;
            int ansIndex = 0;
            if(btn.Name == "ans1")
            {
                this.ans2.Visibility = Visibility.Hidden;
                this.ans3.Visibility = Visibility.Hidden;
                this.ans4.Visibility = Visibility.Hidden;
                ansIndex = 0;
            }
            else if(btn.Name == "ans2")
            {
                this.ans1.Visibility = Visibility.Hidden;
                this.ans3.Visibility = Visibility.Hidden;
                this.ans4.Visibility = Visibility.Hidden;
                ansIndex = 1;
            }
            else if (btn.Name == "ans3")
            {
                this.ans1.Visibility = Visibility.Hidden;
                this.ans2.Visibility = Visibility.Hidden;
                this.ans4.Visibility = Visibility.Hidden;
                ansIndex = 2;
            }
            else
            {
                this.ans1.Visibility = Visibility.Hidden;
                this.ans2.Visibility = Visibility.Hidden;
                this.ans3.Visibility = Visibility.Hidden;
                ansIndex = 3;
            }
            while (time > 0) { }//waits here until timeout
            //then check if this answer is correct with the ansIndex
            if(currectQuestion.correctAns == ansIndex)
            {
                message.Text = "Correct!";
            }
            else
            {
                message.Text = "Wrong";
            }
        }
    }
}
