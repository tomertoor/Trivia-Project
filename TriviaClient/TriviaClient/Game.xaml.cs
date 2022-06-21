using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;
using System.Text.Json;
using System.Threading;
using System.Windows.Input;
using System.Windows.Media;
using System.IO;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for Game.xaml
    /// </summary>
    public partial class Game : Window
    {
        public static User loggedUser;
        private static int timeForQ;
        private static DispatcherTimer Timer;
        private static TimeSpan _time;
        private static int quesCount;
        private static bool isAnswered;
        private static GetQuestionRes currectQuestion;
        private static Thread th;
        private static MediaPlayer theme;
        private static MediaPlayer gong;
        public Game(Window w)
        {
            InitializeComponent();
            this.Left = w.Left;
            this.Top = w.Top;
            AppDomain.CurrentDomain.ProcessExit += (sender, eventArgs) =>
            {
                loggedUser.Logout();
            };
            Room.refresh = false;
            string dir = Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.Parent.FullName;
            loggedUser = Room.loggedUser;
            timeForQ = Room.qTimeout;
            quesCount = Room.quesCount;
            theme = new MediaPlayer();
            theme.Open(new Uri(dir + @"\question.mp3"));
            gong = new MediaPlayer();
            gong.Open(new Uri(dir + @"\gong.mp3"));
            th = new Thread(StartGame);
            th.IsBackground = true;
            th.Start();
        }

        private void StartGame()
        {
            for (int i = 1; i<=quesCount; i++)
            {
                GetQuestion();
                SetTimer();
                isAnswered = false;
                this.Dispatcher.BeginInvoke(new Action(() =>
                {
                    this.ans1.Visibility = Visibility.Visible;
                    this.ans2.Visibility = Visibility.Visible;
                    this.ans3.Visibility = Visibility.Visible;
                    this.ans4.Visibility = Visibility.Visible;
                    this.ans1.IsHitTestVisible = true;
                    this.ans2.IsHitTestVisible = true;
                    this.ans3.IsHitTestVisible = true;
                    this.ans4.IsHitTestVisible = true;
                    this.quesNum.Text = this.quesNum.Text.Substring(0, this.quesNum.Text.LastIndexOf(' ') + 1) + i.ToString();
                }));
                WaitForNextQuestion(_time.Seconds);
                if (!isAnswered)
                {
                    this.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        try
                        {
                            string data = Consts.SUBMIT_ANSWER.PadLeft(2, '0');
                            string msg = "{\"answerId\":-1}";
                            data += msg.Length.ToString().PadLeft(4, '0');
                            data += msg;
                            loggedUser.SendData(data, loggedUser.sock);
                            ServerMsg serverMsg = loggedUser.GetData();
                            SubmitQuestionRes res = new SubmitQuestionRes();
                            switch (serverMsg.code)
                            {
                                case Consts.SUBMIT_ANSWER:
                                    serverMsg.data = serverMsg.data.Remove(0, 1);
                                    serverMsg.data = serverMsg.data.Remove(serverMsg.data.Length - 1, 1);
                                    res = JsonSerializer.Deserialize<SubmitQuestionRes>(serverMsg.data);
                                    break;
                                case Consts.ERROR:
                                    message.Text = serverMsg.data;
                                    break;
                            }
                            if (int.Parse(res.status) == Consts.OK_STATUS)
                            {

                            }
                            else
                            {
                                ErrorResponse errorResponse = new ErrorResponse();
                                errorResponse = JsonSerializer.Deserialize<ErrorResponse>(serverMsg.data);
                                message.Text = errorResponse.message;
                            }
                        }
                        catch (Exception)
                        {
                            message.FontSize = 25;
                            message.Text = "Error occured";
                        }
                    }));
                }
                WaitForNextQuestion(4);//wait 4 seconds between each question
                this.Dispatcher.BeginInvoke(new Action(() =>
                {
                    gong.Stop();
                }));
            }
            this.Dispatcher.BeginInvoke(new Action(() =>
            {
                GameResults results = new GameResults(this);
                this.Close();
                results.Show();
            }));
        }

        private void WaitForNextQuestion(int seconds)
        {
            Thread.Sleep(seconds * 1000);
        }
        
        private void GetQuestion()
        {
            this.Dispatcher.BeginInvoke(new Action(() =>
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
                    else if(res.status.Equals("0"))
                    {
                        GameResults results = new GameResults(this);
                        this.Close();
                        results.Show();
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
            }));
            this.Dispatcher.BeginInvoke(new Action(() =>
            {
                theme.Play();
            }));
        }
        private void AddQuestion()
        {
            this.question.Text = currectQuestion.question;
            this.ans1.Content = currectQuestion.answers[0][1];
            this.ans2.Content = currectQuestion.answers[1][1];
            this.ans3.Content = currectQuestion.answers[2][1];
            this.ans4.Content = currectQuestion.answers[3][1];
        }

        private void SetTimer()
        {
            _time = TimeSpan.FromSeconds(timeForQ);
            Timer = new DispatcherTimer(new TimeSpan(0, 0, 1), DispatcherPriority.Normal, delegate
                {
                    countdown.Text = _time.ToString("c");
                    if (_time == TimeSpan.Zero) { Timer.Stop(); countdown.Text = "Time over!"; this.Dispatcher.BeginInvoke(new Action(() =>{ theme.Stop(); gong.Play();}));};
                    _time = _time.Add(TimeSpan.FromSeconds(-1));
                }, Application.Current.Dispatcher);
            Timer.Start();
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }
        private void OnWindowclose(object sender, EventArgs e)
        {
            try
            {
                loggedUser.Logout();
                th.Abort();
            }
            catch (Exception) { }
        }

        class GetQuestionRes
        {
            public int status { get; set; }
            public string question { get; set; }
            public List<List<JsonElement>> answers { get; set; }
            public GetQuestionRes()
            {
                status = 0;
                question = "";
                answers = new List<List<JsonElement>>();
            }
        }
        class SubmitQuestionRes
        {
            public string status { get; set; }
            public SubmitQuestionRes()
            {
                status = "0";
            }
        }

        private void ans_Click(object sender, RoutedEventArgs e)
        {
            Button btn = (Button)sender;
            int ansIndex = 0;
            if(btn.Name == "ans1")
            {
                this.ans1.IsHitTestVisible = false;
                this.ans2.Visibility = Visibility.Hidden;
                this.ans3.Visibility = Visibility.Hidden;
                this.ans4.Visibility = Visibility.Hidden;
                ansIndex = 0;
            }
            else if(btn.Name == "ans2")
            {
                this.ans2.IsHitTestVisible = false;
                this.ans1.Visibility = Visibility.Hidden;
                this.ans3.Visibility = Visibility.Hidden;
                this.ans4.Visibility = Visibility.Hidden;
                ansIndex = 1;
            }
            else if (btn.Name == "ans3")
            {
                this.ans3.IsHitTestVisible = false;
                this.ans1.Visibility = Visibility.Hidden;
                this.ans2.Visibility = Visibility.Hidden;
                this.ans4.Visibility = Visibility.Hidden;
                ansIndex = 2;
            }
            else
            {
                this.ans4.IsHitTestVisible = false;
                this.ans1.Visibility = Visibility.Hidden;
                this.ans2.Visibility = Visibility.Hidden;
                this.ans3.Visibility = Visibility.Hidden;
                ansIndex = 3;
            }
            isAnswered = true;
            try
            {
                string data = Consts.SUBMIT_ANSWER.PadLeft(2, '0');
                string msg = "{\"answerId\":" + ansIndex + "}";
                data += msg.Length.ToString().PadLeft(4, '0');
                data += msg;
                loggedUser.SendData(data, loggedUser.sock);
                ServerMsg serverMsg = loggedUser.GetData();
                SubmitQuestionRes res = new SubmitQuestionRes();
                switch (serverMsg.code)
                {
                    case Consts.SUBMIT_ANSWER:
                        serverMsg.data = serverMsg.data.Remove(0, 1);
                        serverMsg.data = serverMsg.data.Remove(serverMsg.data.Length - 1, 1);
                        res = JsonSerializer.Deserialize<SubmitQuestionRes>(serverMsg.data);
                        break;
                    case Consts.ERROR:
                        message.Text = serverMsg.data;
                        break;
                }
                if (int.Parse(res.status) == Consts.OK_STATUS)
                {
                    
                }
                else
                {
                    ErrorResponse errorResponse = new ErrorResponse();
                    errorResponse = JsonSerializer.Deserialize<ErrorResponse>(serverMsg.data);
                    message.Text = errorResponse.message;
                }
            }
            catch (Exception)
            {
                message.FontSize = 25;
                message.Text = "Error occured";
            }
        }
    }
}
