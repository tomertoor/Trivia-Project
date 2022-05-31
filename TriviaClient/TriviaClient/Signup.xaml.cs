using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Net.Sockets;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Text.Json;


namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for Signup.xaml
    /// </summary>
    /// 
    public class SignupResponse
    {
        public SignupResponse()
        {
            this.status = "";
        }
        public string status { get; set; }
    }



    public partial class Signup : Window
    {
        public static User loggedUser;
        public Signup()
        {
            InitializeComponent();
        }

        private void register_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                User user = new User();
                user.sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                user.sock.Connect("127.0.0.1", 42069);
                user.username = this.username.Text;
                user.password = this.password.Password;
                user.email = this.email.Text;
                user.phone = this.phone.Text;
                user.birthDate = this.birthDate.Text;
                user.apt = this.apt.Text;
                user.city = this.city.Text;
                user.street = this.street.Text;
                user.Signup();
                ServerMsg msg = user.GetData();
                SignupResponse response = new SignupResponse();
                switch(msg.code)
                {
                    case Consts.SIGN_UP:
                        msg.data = msg.data.Remove(0, 1);
                        msg.data = msg.data.Remove(msg.data.Length - 1, 1);
                        response = JsonSerializer.Deserialize<SignupResponse>(msg.data);
                        break;
                    case Consts.ERROR:
                        this.message.Text = msg.data;

                        break;
                }
               
                if (response.status.Equals("1"))
                {
                    loggedUser = user;
                    loggedUser.passedWhat = Consts.SIGN_UP;
                    Menu menu = new Menu();
                    this.Close();
                    menu.Show();
                }
                
                else
                {
                    ErrorResponse errorResponse = new ErrorResponse();
                    msg.data = msg.data.Remove(0, 1);
                    errorResponse = JsonSerializer.Deserialize<ErrorResponse>(msg.data);

                    this.message.Text = errorResponse.message;

                }
            }
            catch(Exception)
            {
                this.message.FontSize = 25;
                this.message.Text = "Error occured";
            }
        }

        private void home_Click(object sender, RoutedEventArgs e)
        {
            MainWindow home = new MainWindow();
            this.Close();
            home.Show();
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }
    }
}
