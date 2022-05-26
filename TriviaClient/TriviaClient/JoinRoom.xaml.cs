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
    /// Interaction logic for JoinRoom.xaml
    /// </summary>
    public partial class JoinRoom : Window
    {
        private static List<string> rooms;
        private static User loggedUser;
        public JoinRoom()
        {
            InitializeComponent();
            loggedUser = Menu.loggedUser;
            //get available rooms and adds buttons for each one
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseLeftButtonDown(e);
            this.DragMove();
        }

        private void AddButtonsForEachRoom(List<string> updatedRooms)
        {
            //firstly remove all exist buttons
            foreach (var ch in roomButtons.Children)
            {
                if(ch.GetType() == typeof(Button))
                    roomButtons.Children.Remove(ch as Button);
            }

            foreach (string room in updatedRooms) //checks for new rooms
            {
                if (!rooms.Contains(room))
                    rooms.Add(room);
            }
            foreach(string room in rooms) //removes old rooms
            {
                if (!updatedRooms.Contains(room))
                    rooms.Remove(room);
            }

            foreach (string room in rooms)
            {
                Button newButton = new Button();
                newButton.Name = room;
                newButton.Click += joinRoom;
                this.roomButtons.Children.Add(newButton);
            }
        }

        public void joinRoom(object sender, RoutedEventArgs e)
        {
            //join the room by the button name
            Button btn = (Button)sender;
            if(btn != null)
            {
                //request to join to the room and then move to next window
            }
        }
    }
}
