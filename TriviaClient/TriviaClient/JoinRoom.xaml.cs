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
    /// Interaction logic for JoinRoom.xaml
    /// </summary>
    public partial class JoinRoom : Window
    {
        private static List<string> rooms;
        private static User loggedUser;
        public JoinRoom()
        {
            InitializeComponent();
            JoinRoom.rooms = new List<string>();
            while (true)
            {
                loggedUser = Menu.loggedUser;
                //get available rooms and adds buttons for each one
                try
                {
                    string data = Consts.GET_ROOM.PadLeft(2, '0') + "0000";
                    loggedUser.SendData(data, loggedUser.sock);
                    ServerMsg msg = loggedUser.GetData();
                    GetroomsResponse res = new GetroomsResponse();
                    switch (msg.code)
                    {
                        case Consts.GET_ROOM:
                            /*msg.data = msg.data.Remove(0, 1);
                            msg.data = msg.data.Remove(msg.data.Length - 1, 1);*/
                            res = JsonSerializer.Deserialize<GetroomsResponse>(msg.data);
                            break;
                        case Consts.ERROR:
                            this.message.Text = msg.data;
                            break;
                    }
                    if (res.status == Consts.OK_STATUS)
                    {
                        AddButtonsForEachRoom(res.rooms);
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
                Thread thread = new Thread(SleepThread);
                thread.Join();
           }
        }

        private void SleepThread()
        {
            Thread.Sleep(3000);
        }
        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        private void AddButtonsForEachRoom(List<string> updatedRooms)
        {
            //firstly remove all exist buttons
            foreach (var ch in roomButtons.Children)
            {
                if(ch.GetType() == typeof(Button))
                    roomButtons.Children.Remove(ch as Button);
            }

            foreach (string room in updatedRooms) //checks for new rooms
            {
                if (!rooms.Contains(room))
                    rooms.Add(room);
            }
            foreach(string room in rooms) //removes old rooms
            {
                if (!updatedRooms.Contains(room))
                    rooms.Remove(room);
            }

            foreach (string room in rooms)
            {
                Button newButton = new Button();
                newButton.Name = room;
                newButton.Content = room;
                newButton.Click += joinRoom;
                newButton.Width = 160;
                newButton.Height = 55;
                newButton.Margin = new Thickness(5);
                this.roomButtons.Children.Add(newButton);
            }
        }

        public void joinRoom(object sender, RoutedEventArgs e)
        {
            Button btn = (Button)sender;
            if(btn != null)
            {
                try
                {
                    string data = Consts.JOIN_ROOM.PadLeft(2, '0');
                    string m = "{\"roomName\": \"" + rooms.IndexOf(btn.Name) + "\"}";
                    data += m.Length.ToString().PadLeft(4, '0');
                    data += m;
                    loggedUser.SendData(data, loggedUser.sock);
                    ServerMsg msg = loggedUser.GetData();
                    JoinroomResponse res = new JoinroomResponse();
                    switch (msg.code)
                    {
                        case Consts.PERSONAL_STATS:
                            msg.data = msg.data.Remove(0, 1);
                            msg.data = msg.data.Remove(msg.data.Length - 1, 1);
                            res = JsonSerializer.Deserialize<JoinroomResponse>(msg.data);
                            break;
                        case Consts.ERROR:
                            this.message.Text = msg.data;
                            break;
                    }
                    if (res.status.Equals("1"))
                    {
                        //move to the room window
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
        }

        private void menu_Click(object sender, RoutedEventArgs e)
        {
            Menu menu = new Menu();
            this.Close();
            menu.Show();
        }
    }

    public class GetroomsResponse
    {
        public int status { get; set; }
        public List<string> rooms { get; set; }
        public GetroomsResponse()
        {
            status = 0;
        }
    }

    public class JoinroomResponse
    {
        public string status;
        public JoinroomResponse()
        {
            status = "";
        }
    }
}
