using System;
using System.Windows;
using System.Windows.Input;
using System.Net.Sockets;
using System.Text.Json;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for Login.xaml
    /// </summary>
    /// 
    public partial class Login : Window
    {
        public static User loggedUser;
        //constructor
        public Login(Window w)
        {
            InitializeComponent();
            this.Left = w.Left;
            this.Top = w.Top;
            AppDomain.CurrentDomain.ProcessExit += (sender, eventArgs) =>
            {
                loggedUser.Logout();
            };
        }

        //function for handling home button click
        private void home_Click(object sender, RoutedEventArgs e)
        {
            MainWindow main = new MainWindow();
            this.Close();
            main.Show();
        }

        //function for handling the login button click
        private void Login_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                User user = new User();
                PAZCryptoAlgorithm algorithm = new PAZCryptoAlgorithm("");
                user.sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                user.sock.Connect(Consts.IP, Consts.PORT);
                user.username = this.username.Text;
                user.password = this.password.Password;
                //creates new user, adding the data the user gave
                user.Login();
                ServerMsg msg = user.GetData();
                LoginResponse response = new LoginResponse();
                switch (msg.code)
                {
                    case Consts.LOG_IN:
                        this.message.Text = msg.data;
                        msg.data = msg.data.Remove(0, 1);
                        msg.data = msg.data.Remove(msg.data.Length - 1, 1);
                        response = JsonSerializer.Deserialize<LoginResponse>(msg.data);
                        break;
                    case Consts.ERROR:
                        this.message.Text = msg.data;
                        break;
                }
                if (!response.status.Equals("1"))
                {
                    ErrorResponse errorResponse = new ErrorResponse();
                    msg.data = msg.data.Remove(0, 1);
                    errorResponse = JsonSerializer.Deserialize<ErrorResponse>(msg.data);
                    this.message.Text = errorResponse.message;
                }
                else
                {
                    //in case of success, pass the logged user to the next window, menu
                    loggedUser = user;
                    loggedUser.passedWhat = Consts.LOG_IN;
                    Menu menu = new Menu(this);
                    this.Close();
                    menu.Show();
                }
            }
            catch(Exception)
            {
                this.message.FontSize = 25;
                this.message.Text = "Error occured";
            }
        }

        //to enable moveing the window throug the screen
        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }
    }
}
