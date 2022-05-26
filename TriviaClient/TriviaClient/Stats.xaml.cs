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

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for Stats.xaml
    /// </summary>
    public partial class Stats : Window
    {
        public static User loggedUser;
        public Stats()
        {
            InitializeComponent();
            loggedUser = Menu.loggedUser;
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        private void highScores_Click(object sender, RoutedEventArgs e)
        {
            HighScores scores = new HighScores();
            this.Close();
            scores.Show();
        }

        private void menu_Click(object sender, RoutedEventArgs e)
        {
            Menu menu = new Menu();
            this.Close();
            menu.Show();
        }

        private void myStats_Click(object sender, RoutedEventArgs e)
        {
            UserStats stats = new UserStats();
            this.Close();
            stats.Show();
        }
    }
}
