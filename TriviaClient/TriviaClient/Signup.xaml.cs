using System;
using System.Windows;
using System.Windows.Input;
using System.Net.Sockets;
using System.Text.Json;


namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for Signup.xaml
    /// </summary>
    /// 
    
    public partial class Signup : Window
    {
        public static User loggedUser;
        public Signup(Window w)
        {
            InitializeComponent();
            this.Left = w.Left;
            this.Top = w.Top;
            AppDomain.CurrentDomain.ProcessExit += (sender, eventArgs) =>
            {
                string data = Consts.LOG_OUT.PadLeft(2, '0') + "0000";
                loggedUser.SendData(data, loggedUser.sock);
            };
        }

        private void register_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                User user = new User();
                user.sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                user.sock.Connect(Consts.IP, 42069);
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
                    Menu menu = new Menu(this);
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
