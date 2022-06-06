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
        private static bool hasGameBegun;
        public Room()
        {
            InitializeComponent();
            users = new List<string>();
            if(JoinRoom.loggedUser.passedWhat == Consts.JOIN_ROOM)
            {
                loggedUser = JoinRoom.loggedUser;
                mod = Consts.MEMBER;
                this.myGrid.Children.Remove(this.start);
            }
            if(CreateRoom.loggedUser.passedWhat == Consts.CREATE_ROOM)
            {
                loggedUser = CreateRoom.loggedUser;
                mod = Consts.ADMIN;
            }
            Thread thread = new Thread(RefreshUsers);
            thread.Start();
            ;
        }

        private void RefreshUsers()
        {
            while (true)
            {
                try
                {
                    string data = Consts.GET_PLAYERS.PadLeft(2, '0') + "0000";
                    loggedUser.SendData(data, loggedUser.sock);
                    ServerMsg msg = loggedUser.GetData();
                    GetRoomStateResponse res = new GetRoomStateResponse();
                    switch (msg.code)
                    {
                        case Consts.GET_ROOM:
                            res = JsonSerializer.Deserialize<GetRoomStateResponse>(msg.data);
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
                        AddLabelsForUsers(res.playersInRoom);
                        this.timePerQ.Text += " " + res.answerTimeOut.ToString();
                        this.qCount.Text += " " + res.answerCount.ToString();
                        hasGameBegun = res.hasGameBegun;
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
                for (int i = 0; i < players.Children.Count; i++)
                {
                    if (players.Children[i].GetType() == typeof(Label))
                        players.Children.Remove(players.Children[i] as Label);
                }

                foreach (string user in updatedUsers) //checks for new rooms
                {
                    if (!users.Contains(user))
                        users.Add(user);
                }
                foreach (string user in users) //removes old rooms
                {
                    if (!updatedUsers.Contains(user))
                        users.Remove(user);
                }

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
            Menu menu = new Menu();
            this.Close();
            menu.Show();
        }

        class GetRoomStateResponse
        {
            public int status { get; set; }
            public bool hasGameBegun { get; set; }
            public List<string> playersInRoom { get; set; }
            public int answerCount { get; set; }
            public int answerTimeOut { get; set; }
            public GetRoomStateResponse()
            {
                status = 0;
                hasGameBegun = false;
                playersInRoom = new List<string>();
                answerCount = 0;
                answerTimeOut = 0;
            }
        }
    }
}
