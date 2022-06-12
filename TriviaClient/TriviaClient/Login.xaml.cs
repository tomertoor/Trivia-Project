﻿using System;
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
        public Login(Window w)
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
                user.sock.Connect(Consts.IP, 42069);
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

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }
    }
}
