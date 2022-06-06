using System;
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

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for CreateRoom.xaml
    /// </summary>
    public partial class CreateRoom : Window
    {
        private static User loggedUser;
        public CreateRoom()
        {
            InitializeComponent();
            loggedUser = Menu.loggedUser;
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        private void menu_Click(object sender, RoutedEventArgs e)
        {
            Menu menu = new Menu();
            this.Close();
            menu.Show();
        }

        private void create_Click(object sender, RoutedEventArgs e)
        {
            //send request by the info from the user
            //in case of error checnge the 'message.Text'
            try
            {
                var createRoom = new CreateRoomRequest
                {
                    roomName = this.roomName.Text,
                    maxUsers = this.noOfPlayers.Text,
                    questionTimeout = this.timeForQ.Text,
                    questionCount = this.questionCount.Text
                };
                string data = Consts.CREATE_ROOM.PadLeft(2, '0');
                string msg = JsonSerializer.Serialize(createRoom);
                data += msg.Length.ToString().PadLeft(4, '0');
                data += msg;
                loggedUser.SendData(data, loggedUser.sock);
                ServerMsg response = loggedUser.GetData();
                CreateRoomResponse res = new CreateRoomResponse();
                switch(response.code)
                {
                    case Consts.CREATE_ROOM:
                        response.data = response.data.Remove(0, 1);
                        response.data = response.data.Remove(response.data.Length - 1, 1);
                        res = JsonSerializer.Deserialize<CreateRoomResponse>(response.data);
                        break;
                    case Consts.ERROR:
                        this.message.Text = response.data;
                        break;
                }
                if(res.status.Equals("1"))
                {
                    //move to the next window, the room window
                }
                else
                {
                    ErrorResponse errorResponse = new ErrorResponse();
                    response.data = response.data.Remove(0, 1);
                    errorResponse = JsonSerializer.Deserialize<ErrorResponse>(response.data);

                    this.message.Text = errorResponse.message;
                }
            }
            catch(Exception)
            {
                this.message.FontSize = 25;
                this.message.Text = "Error occured";
            }
        }
    }

    public class CreateRoomRequest
    {
        public string questionTimeout { get; set; }
        public string roomName { get; set; }
        public string maxUsers { get; set; }

        public string questionCount { get; set; }
    }
    public class CreateRoomResponse
    {
        public CreateRoomResponse()
        {
            this.status = "";
        }
        public string status { get; set; }
    }
}
