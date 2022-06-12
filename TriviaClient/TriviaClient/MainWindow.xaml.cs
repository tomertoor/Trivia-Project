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
        public MainWindow()
        {
            InitializeComponent();
            
            this.LogIn.FontSize = 14;
        }
        private void LogIn_Click(object sender, RoutedEventArgs e)
        {
            Login log = new Login(this);
            this.Close();
            log.Show();
        }

        private void Signup_Click(object sender, RoutedEventArgs e)
        {
            Signup sign = new Signup(this);
            this.Close();
            sign.Show();
        }

        private void info_Click(object sender, RoutedEventArgs e)
        {
            Info info = new Info(this);
            this.Close();
            info.Show();
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        private void exit_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
