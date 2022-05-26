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
    /// Interaction logic for CreateRoom.xaml
    /// </summary>
    public partial class CreateRoom : Window
    {
        private static User loggedUser;
        public CreateRoom()
        {
            InitializeComponent();
            loggedUser = Menu.loggedUser;
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        private void menu_Click(object sender, RoutedEventArgs e)
        {
            Menu menu = new Menu();
            this.Close();
            menu.Show();
        }

        private void create_Click(object sender, RoutedEventArgs e)
        {
            //send request by the info from the user
            //in case of error checnge the 'message.Text'
        }
    }
}
