using System.Windows;
using System.Windows.Input;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for Menu.xaml
    /// </summary>
    public partial class Menu : Window
    {
        public static User loggedUser;
        public Menu()
        {
            InitializeComponent();
            if (Login.loggedUser.passedWhat == Consts.LOG_IN)
                loggedUser = Login.loggedUser;
            else if (Signup.loggedUser.passedWhat == Consts.SIGN_UP)
                loggedUser = Signup.loggedUser;
            else if (UserStats.loggedUser.passedWhat == Consts.PERSONAL_STATS)
                loggedUser = UserStats.loggedUser;
            else if (HighScores.loggedUser.passedWhat == Consts.HIGH_SCORES)
                loggedUser = HighScores.loggedUser;
            else if (Stats.loggedUser.passedWhat == Consts.STATS)
                loggedUser = Stats.loggedUser;
            else if (Room.loggedUser.passedWhat == Consts.ADMIN || Room.loggedUser.passedWhat == Consts.MEMBER)
                loggedUser = Room.loggedUser;
            else
            {
                loggedUser.username = "none";
                loggedUser.password = "none";
            }
            this.username.Text = "Hello " + loggedUser.username + "!";
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        private void exit_Click(object sender, RoutedEventArgs e)
        {
            loggedUser.Logout();
            ServerMsg msg = loggedUser.GetData();
            this.Close();
        }

        private void createRoom_Click(object sender, RoutedEventArgs e)
        {
            CreateRoom create = new CreateRoom();
            this.Close();
            create.Show();
        }

        private void joinRoom_Click(object sender, RoutedEventArgs e)
        {
            JoinRoom join = new JoinRoom();
            this.Close();
            join.Show();
        }

        private void statistics_Click(object sender, RoutedEventArgs e)
        {
            Stats stats = new Stats();
            this.Close();
            stats.Show();
        }
    }
}
