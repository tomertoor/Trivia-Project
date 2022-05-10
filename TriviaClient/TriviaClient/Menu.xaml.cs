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
    /// Interaction logic for Menu.xaml
    /// </summary>
    public partial class Menu : Window
    {
        private static User loggedUser;
        public Menu()
        {
            InitializeComponent();
            if(Login.loggedUser.passedWhat == Consts.LOG_IN)
                loggedUser = Login.loggedUser;
            else if (Signup.loggedUser.passedWhat == Consts.SIGN_UP)
                loggedUser = Signup.loggedUser;
            else
            {
                loggedUser.username = "none";
                loggedUser.password = "none";
            }
            this.password.Text = loggedUser.password;
            this.username.Text = loggedUser.username;
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }
    }
}
