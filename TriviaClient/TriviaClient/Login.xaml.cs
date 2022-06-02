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
        public Login()
        {
            InitializeComponent();
        }

        private void home_Click(object sender, RoutedEventArgs e)
        {
            MainWindow main = new MainWindow();
            this.Close();
            main.Show();
        }
        private void Login_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                User user = new User();
                user.sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                user.sock.Connect("127.0.0.1", 42069);
                user.username = this.username.Text;
                user.password = this.password.Password;
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
                    loggedUser = user;
                    loggedUser.passedWhat = Consts.LOG_IN;
                    Menu menu = new Menu();
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

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }
    }
}
