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

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for Login.xaml
    /// </summary>
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
            User user = new User();
            user.sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            user.sock.Connect("127.0.0.1", 42069);
            user.username = this.username.Text;
            user.password = this.password.Password;
            user.Login();
            ServerMsg msg = user.GetData();
            if (msg.code == Consts.ERROR)
                this.message.Text = msg.data;
            else
            {
                loggedUser = user;
                loggedUser.passedWhat = Consts.LOG_IN;
                Menu menu = new Menu();
                this.Close();
                menu.Show();
            }
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }
    }
}
