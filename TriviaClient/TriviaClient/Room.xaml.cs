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
        private static bool hasGameBegun;
        private static bool refresh;
        private static int quesCount;
        private static int qTimeout;
        public Room()
        {
            InitializeComponent();
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
                mod = Consts.ADMIN;
            }
            this.level.Text += mod;
            refresh = true;
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

        private void RefreshUsers()
        {
            while (refresh)
            {
                try
                {
                    string data = Consts.GET_ROOM_STATE.PadLeft(2, '0') + "0000";
                    loggedUser.SendData(data, loggedUser.sock);
                    ServerMsg msg = loggedUser.GetData();
                    GetRoomStateResponse res = new GetRoomStateResponse();
                    GetClosedroomResponse close = new GetClosedroomResponse();
                    switch (msg.code)
                    {
                        case Consts.GET_ROOM_STATE:
                            res = JsonSerializer.Deserialize<GetRoomStateResponse>(msg.data);
                            break;
                        case Consts.LEAVE_ROOM:
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
                            }
                            if (qTimeout != res.answerTimeout)
                            {
                                this.timePerQ.Text = this.timePerQ.Text.Substring(0, this.timePerQ.Text.LastIndexOf(':') + 1) + " " + res.answerTimeout.ToString();
                            }
                            hasGameBegun = res.hasGameBegun;
                        }));
                    }
                    else if (close.status == Consts.OK_STATUS)
                    {
                        refresh = false;
                        loggedUser.passedWhat = mod;
                        Menu menu = new Menu();
                        this.Close();
                        menu.Show();
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
                Thread.Sleep(3000);
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
            //start game
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
            Menu menu = new Menu();
            this.Close();
            menu.Show();
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
            public int status { get; set; }
            public GetClosedroomResponse()
            {
                status = 0;
            }
        }
    }
}
