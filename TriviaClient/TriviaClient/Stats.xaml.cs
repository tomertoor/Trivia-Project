﻿using System.Windows;
using System.Windows.Input;

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
            loggedUser.passedWhat = Consts.STATS;
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
