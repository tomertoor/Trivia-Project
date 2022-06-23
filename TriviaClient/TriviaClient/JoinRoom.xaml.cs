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
        public static User loggedUser;
        private static Thread thread;
        public static bool refresh;
        public JoinRoom(Window w)
        {
            InitializeComponent();
            this.Left = w.Left;
            this.Top = w.Top;
            loggedUser = Menu.loggedUser;
            AppDomain.CurrentDomain.ProcessExit += new EventHandler(OnProcessExit);
            JoinRoom.rooms = new List<string>();
            thread = new Thread(RefreshRooms);
            refresh = true;
            thread.IsBackground = true;
            thread.Start();
            ;
        }

        /*Function responsible on refreshing room async
         * Input - None
         * Output - None
         */
        private void RefreshRooms()
        {
            while (refresh)
            {
                try
                {
                    string data = Consts.GET_ROOM.PadLeft(2, '0') + "0000";
                    loggedUser.SendData(data, loggedUser.sock);
                    ServerMsg msg = loggedUser.GetData();
                    GetroomsResponse res = new GetroomsResponse();
                    switch (msg.code)
                    {
                        case Consts.GET_ROOM:
                            res = JsonSerializer.Deserialize<GetroomsResponse>(msg.data);
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
                        AddButtonsForEachRoom(res.rooms);
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
                Thread.Sleep(1500);
            }
        }
        //Responsible on dragging
        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        private void OnProcessExit(object sender, EventArgs e)
        {
            loggedUser.Logout();
        }
        /*Function responsible on represnting each room with buttons 
         * Input - the list of room names
         * Output - None
         */
        private void AddButtonsForEachRoom(List<string> updatedRooms)
        {
            this.Dispatcher.BeginInvoke(new Action(() =>
            {
                //firstly remove all exist buttons
                roomButtons.Children.Clear();

                rooms = updatedRooms;

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
            }));
        }

        /*Callback function for joinRoom, uses button index in the list to join
         Input - the object that trigerred the event and the eventArgs if any
        Output - None. 
        */
        public void joinRoom(object sender, RoutedEventArgs e)
        {
            Button btn = (Button)sender;
            if(btn != null)
            {
                refresh = false;
                try
                {
                    string data = Consts.JOIN_ROOM.PadLeft(2, '0');
                    string m = "{\"roomId\": " + rooms.IndexOf(btn.Name) + "}";
                    data += m.Length.ToString().PadLeft(4, '0');
                    data += m;
                    loggedUser.SendData(data, loggedUser.sock);
                    ServerMsg msg = loggedUser.GetData();
                    JoinroomResponse res = new JoinroomResponse();
                    switch (msg.code)
                    {
                        case Consts.JOIN_ROOM:
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
                        try
                        {
                            thread.Abort();
                        }
                        catch (Exception)
                        {
                            refresh = false;
                            loggedUser.passedWhat = Consts.JOIN_ROOM;
                            Room.name = btn.Name;
                            Room room = new Room(this);
                            this.Close();
                            room.Show();
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
                refresh = true;
                try
                {
                    thread.IsBackground = true;
                    thread.Start();
                    ;
                }
                catch(Exception)
                {
                    //fjj
                }
            }
        }

        private void menu_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                refresh = false;
                thread.Abort();
            }
            catch (Exception)
            {
                Menu menu = new Menu(this);
                this.Close();
                menu.Show();
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            refresh = false;
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
        public string status { get; set; }
        public JoinroomResponse()
        {
            status = "";
        }
    }
}
