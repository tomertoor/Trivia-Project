using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
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
using System.Text.Json;

namespace TriviaClient
{
    public struct User
    {
        public Socket sock;
        public string username;
        public string password;
        public string email;
        public string phone;
        public string birthDate;
        public string apt;
        public string city;
        public string street;
        public string passedWhat;

        public void Login()
        {
            string data = Consts.LOG_IN;
            string msg = "{\"username\": \"" + this.username + "\", \"password\": \"" + this.password + "\"}";
            data += msg.Length.ToString().PadLeft(4, '0');
            data += msg;
            SendData(data, this.sock);
        }

        public void Signup()
        {
            string data = Consts.SIGN_UP;
            string msg = "{\"username\": \"" + this.username + "\", \"password\": \"" + this.password + "\", \"email\": \"" + this.email + "\"" +
                ", \"phone\": \"" + this.phone + "\", \"birthDate\": \"" + this.birthDate + "\", \"apt\": \"" + this.apt + "\"" +
                ", \"city\": \"" + this.city + "\", \"street\": \"" + this.street + "\"}";
            data += msg.Length.ToString().PadLeft(4, '0');
            data += msg;
            SendData(data, this.sock);
        }

        public static void SendData(string data, Socket sock)
        {
            byte[] msg = new byte[data.Length];
            for (int i = 0; i < data.Length; i++)
                msg[i] = (byte)data[i];
            sock.Send(msg);
        }

        public ServerMsg GetData()
        {
            ServerMsg msg;
            byte[] code = new byte[5];
            byte[] c = new byte[1];
            this.sock.Receive(code, 5, 0);
            int size = Int32.Parse(Encoding.ASCII.GetString(code, 1, 4));
            byte[] data = new byte[size];
            this.sock.Receive(data, size, 0);
            c[0] = code[0];
            msg.code = Encoding.ASCII.GetString(c);
            msg.data = Encoding.ASCII.GetString(data);
            return msg;
        }
    }

    class Consts
    {
        public const string LOG_IN = "5";
        public const string SIGN_UP = "1";
        public const string ERROR = "3";
    }

    public struct ServerMsg
    {
        public string code;
        public string data;
    }
}
