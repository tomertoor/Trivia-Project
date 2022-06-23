using System;
using System.Windows;
using System.Windows.Input;
using System.Text.Json;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for CreateRoom.xaml
    /// </summary>
    public partial class CreateRoom : Window
    {
        public static User loggedUser;
        public static string name;
        public CreateRoom(Window w)
        {
            InitializeComponent();
            this.Left = w.Left;
            this.Top = w.Top;
            loggedUser = Menu.loggedUser;
            AppDomain.CurrentDomain.ProcessExit += (sender, eventArgs) =>
            {
                string data = Consts.LOG_OUT.PadLeft(2, '0') + "0000";
                loggedUser.SendData(data, loggedUser.sock);
            };
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        private void menu_Click(object sender, RoutedEventArgs e)
        {
            Menu menu = new Menu(this);
            this.Close();
            menu.Show();
        }
        /*function that is responsible on handling the create button click event, sends packet to the server to create the room
         */
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
                if(res.status.Equals("1")) //if the room is succesfully made
                {
                    name = this.roomName.Text;
                    loggedUser.passedWhat = Consts.CREATE_ROOM;
                    Room room = new Room(this);
                    this.Close();
                    room.Show();
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
