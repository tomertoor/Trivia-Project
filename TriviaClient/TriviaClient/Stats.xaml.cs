using System;
using System.Windows;
using System.Windows.Input;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for Stats.xaml
    /// </summary>
    public partial class Stats : Window
    {
        public static User loggedUser;
        
        //constructor
        public Stats(Window w)
        {
            InitializeComponent();
            this.Left = w.Left;
            this.Top = w.Top;
            AppDomain.CurrentDomain.ProcessExit += (sender, eventArgs) =>
            {
                loggedUser.Logout();
            };
            loggedUser = Menu.loggedUser;
        }

        //to enable moving the window throug the screen
        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        //function to handle the high scores button click
        private void highScores_Click(object sender, RoutedEventArgs e)
        {
            HighScores scores = new HighScores(this);
            this.Close();
            scores.Show();
        }

        //function to handle the return to menu button click
        private void menu_Click(object sender, RoutedEventArgs e)
        {
            loggedUser.passedWhat = Consts.STATS;
            Menu menu = new Menu(this);
            this.Close();
            menu.Show();
        }

        //function to handle my stats button click
        private void myStats_Click(object sender, RoutedEventArgs e)
        {
            UserStats stats = new UserStats();
            this.Close();
            stats.Show();
        }
    }
}
