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

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for Signup.xaml
    /// </summary>
    public partial class Signup : Window
    {
        public static User loggedUser;
        public Signup()
        {
            InitializeComponent();
        }

        private void register_Click(object sender, RoutedEventArgs e)
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
            if (msg.code == Consts.ERROR)
                this.message.Text = msg.data;
            else
            {
                loggedUser = user;
                loggedUser.passedWhat = Consts.SIGN_UP;
                Menu menu = new Menu();
                this.Close();
                menu.Show();
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
