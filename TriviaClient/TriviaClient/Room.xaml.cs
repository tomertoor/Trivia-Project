using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Text.Json;
using System.Threading;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for Room.xaml
    /// </summary>
    public partial class Room : Window
    {
        private static List<string> users;
        public static User loggedUser;
        private static string mod;
        public static string name;
        public static bool refresh;
        private static Mutex mut;
        public static int quesCount;
        public static int qTimeout;
        public Room(Window w)
        {
            InitializeComponent();
            this.Left = w.Left;
            this.Top = w.Top;
            AppDomain.CurrentDomain.ProcessExit += (sender, eventArgs) =>
            {
                loggedUser.Logout();
            };
            JoinRoom.refresh = false;
            users = new List<string>();
            if (JoinRoom.loggedUser.passedWhat == Consts.JOIN_ROOM)
            {
                loggedUser = JoinRoom.loggedUser;
                mod = Consts.MEMBER;
                this.myGrid.Children.Remove(this.start);
            }
            if (CreateRoom.loggedUser.passedWhat == Consts.CREATE_ROOM)
            {
                loggedUser = CreateRoom.loggedUser;
                name = CreateRoom.name;
                mod = Consts.ADMIN;
                this.@return.Content = "Close Room";
            }
            this.level.Text += mod;
            refresh = true;
            mut = new Mutex();
            roomName.Text += " " + name;
            quesCount = 0;
            qTimeout = 0;
            Thread thread = new Thread(RefreshUsers);
            thread.Start();
            ;
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        private void OnWindowclose(object sender, EventArgs e)
        {
            loggedUser.Logout();
            refresh = false;
        }
        private void RefreshUsers()
        {
            while (refresh)
            {
                try
                {
                    string data = Consts.GET_ROOM_STATE.PadLeft(2, '0') + "0000";
                    mut.WaitOne();
                    loggedUser.SendData(data, loggedUser.sock);
                    ServerMsg msg = loggedUser.GetData();
                    mut.ReleaseMutex();
                    GetRoomStateResponse res = new GetRoomStateResponse();
                    GetClosedroomResponse close = new GetClosedroomResponse();
         
                    switch (msg.code)
                    {
                        case Consts.GET_ROOM_STATE:
                            res = JsonSerializer.Deserialize<GetRoomStateResponse>(msg.data);
                            break;
                        case Consts.LEAVE_ROOM:
                            msg.data = msg.data.Remove(0, 1);
                            msg.data = msg.data.Remove(msg.data.Length - 1, 1);
                            close = JsonSerializer.Deserialize<GetClosedroomResponse>(msg.data);
                            break;
                        case Consts.ERROR:
                            this.Dispatcher.BeginInvoke(new Action(() =>
                            {
                                message.Text = msg.data;
                            }));
                            break;
                    }
                    if (res.status == Consts.OK_STATUS)
                    {
                        this.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            AddLabelsForUsers(res.players);
                            if (quesCount != res.questionCount)
                            {
                                this.qCount.Text = this.qCount.Text.Substring(0, this.qCount.Text.LastIndexOf(':') + 1) + " " + res.questionCount.ToString();
                                quesCount = res.questionCount;
                            }
                            if (qTimeout != res.answerTimeout)
                            {
                                this.timePerQ.Text = this.timePerQ.Text.Substring(0, this.timePerQ.Text.LastIndexOf(':') + 1) + " " + res.answerTimeout.ToString();
                                qTimeout = res.answerTimeout;
                            }
                            if(res.hasGameBegun)
                            {
                                refresh = false;
                                Game game = new Game(this);
                                this.Close();
                                game.Show();
                            }
                        }));
                    }
                    else if (int.Parse(close.status) == Consts.OK_STATUS)
                    {
                            this.Dispatcher.Invoke((Action)delegate {
                            refresh = false;
                            loggedUser.passedWhat = mod;
                            Menu menu = new Menu(this);
                            this.Close();
                            menu.Show();
                        });
                    }
                    else
                    {
                        ErrorResponse errorResponse = new ErrorResponse();
                        errorResponse = JsonSerializer.Deserialize<ErrorResponse>(msg.data);
                        this.Dispatcher.BeginInvoke(new Action(() =>
                        {
                            message.Text = errorResponse.message;
                        }));
                    }
                }
                catch (Exception)
                {
                    this.Dispatcher.BeginInvoke(new Action(() =>
                    {
                        message.FontSize = 25;
                        message.Text = "Error occured";
                    }));
                }
                Thread.Sleep(500);
            }
        }

        private void AddLabelsForUsers(List<string> updatedUsers)
        {
            this.Dispatcher.BeginInvoke(new Action(() =>
            {
                players.Children.Clear();
                
                users = updatedUsers;
                foreach (string room in users)
                {
                    Label newLabel = new Label();
                    newLabel.Content = room;
                    newLabel.FontSize = 20;
                    this.players.Children.Add(newLabel);
                }
            }));
        }
        private void start_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                string data = Consts.START_GAME + "0000";
                mut.WaitOne();
                loggedUser.SendData(data, loggedUser.sock);
                ServerMsg msg = loggedUser.GetData();
                mut.ReleaseMutex();
                GetStartGameRepsonse response = new GetStartGameRepsonse();
                switch (msg.code)
                {
                    case Consts.START_GAME:
                        msg.data = msg.data.Remove(0, 1);
                        msg.data = msg.data.Remove(msg.data.Length - 1, 1);
                        response = JsonSerializer.Deserialize<GetStartGameRepsonse>(msg.data);
                        break;
                    case Consts.ERROR:
                        this.message.Text = msg.data;
                        break;
                }

                if (response.status.Equals("1"))
                {
                    refresh = false;
                    Game game = new Game(this);
                    this.Close();
                    game.Show();
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

        private void return_Click(object sender, RoutedEventArgs e)
        {
            loggedUser.passedWhat = mod;
            refresh = false;
            if (mod == Consts.ADMIN)
            {
                string data = Consts.CLOSE_ROOM.PadLeft(2, '0') + "0000";
                loggedUser.SendData(data, loggedUser.sock);
                ServerMsg msg = loggedUser.GetData();
            }
            if (mod == Consts.MEMBER)
            {
                string data = Consts.LEAVE_ROOM.PadLeft(2, '0') + "0000";
                loggedUser.SendData(data, loggedUser.sock);
                ServerMsg msg = loggedUser.GetData();
            }
            Menu menu = new Menu(this);
            this.Close();
            menu.Show();
        }

        class GetStartGameRepsonse
        {
            public string status { get; set; }
            public GetStartGameRepsonse()
            {
                status = "";
            }
        }
        class GetRoomStateResponse
        {
            public int status { get; set; }
            public bool hasGameBegun { get; set; }
            public List<string> players { get; set; }
            public int questionCount { get; set; }
            public int answerTimeout { get; set; }
            public GetRoomStateResponse()
            {
                status = 0;
                hasGameBegun = false;
                players = new List<string>();
                questionCount = 0;
                answerTimeout = 0;
            }
        }

        class GetClosedroomResponse
        {
            public string status { get; set; }
            public GetClosedroomResponse()
            {
                status = "0";
            }
        }
    }
}
