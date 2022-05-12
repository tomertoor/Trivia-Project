using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

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
            Login log = new Login();
            this.Close();
            log.Show();
        }

        private void Signup_Click(object sender, RoutedEventArgs e)
        {
            Signup sign = new Signup();
            this.Close();
            sign.Show();
        }

        private void info_Click(object sender, RoutedEventArgs e)
        {
            Info info = new Info();
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
