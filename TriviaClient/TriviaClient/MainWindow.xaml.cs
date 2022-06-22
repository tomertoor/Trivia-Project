using System;
using System.Windows;
using System.Windows.Input;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //constructor
        public MainWindow()
        {
            InitializeComponent();
            
            this.LogIn.FontSize = 14;
        }

        //function for handling login button click
        private void LogIn_Click(object sender, RoutedEventArgs e)
        {
            Login log = new Login(this);
            this.Close();
            log.Show();
        }

        //function for handling signup button click
        private void Signup_Click(object sender, RoutedEventArgs e)
        {
            Signup sign = new Signup(this);
            this.Close();
            sign.Show();
        }

        //function for handling info button click
        private void info_Click(object sender, RoutedEventArgs e)
        {
            Info info = new Info(this);
            this.Close();
            info.Show();
        }
        //to enable moving the window throug the screen
        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        //exiting the program
        private void exit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
