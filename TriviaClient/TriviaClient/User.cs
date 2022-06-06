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
    public class SignupResponse
    {
        public SignupResponse()
        {
            this.status = "";
        }
        public string status { get; set; }
    }

    public class LoginResponse
    {
        public LoginResponse()
        {
            this.status = "";
        }
        public string status { get; set; }
    }

    public class ErrorResponse
    {
        public ErrorResponse()
        {
            this.message = "";
        }
        public string message { get; set; }
    }
    class Consts
    {
        public const string STATS = "s";
        public const int OK_STATUS = 1;
        public const string SIGN_UP = "01";
        public const string LOG_OUT = "02";
        public const string ERROR = "03";
        public const string CREATE_ROOM = "04";
        public const string LOG_IN = "05";
        public const string JOIN_ROOM = "06";
        public const string GET_ROOM = "07";
        public const string PERSONAL_STATS = "08";
        public const string GET_PLAYERS = "09";
        public const string HIGH_SCORES = "10";
    }
    
    public struct Message
    {
        string code;
        string data;
    }
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
            string data = Consts.LOG_IN.PadLeft(2, '0');
            string msg = "{\"username\": \"" + this.username + "\", \"password\": \"" + this.password + "\"}";
            data += msg.Length.ToString().PadLeft(4, '0');
            data += msg;
            SendData(data, this.sock);
        }

        public void Signup()
        {
            string data = Consts.SIGN_UP.PadLeft(2, '0');
            string msg = "{\"username\": \"" + this.username + "\", \"password\": \"" + this.password + "\", \"email\": \"" + this.email + "\"" +
                ", \"phone\": \"" + this.phone + "\", \"birthDate\": \"" + this.birthDate + "\", \"apt\": \"" + this.apt + "\"" +
                ", \"city\": \"" + this.city + "\", \"street\": \"" + this.street + "\"}";
            data += msg.Length.ToString().PadLeft(4, '0');
            data += msg;
            SendData(data, this.sock);
        }

        public void Logout()
        {
            string data = Consts.LOG_OUT.PadLeft(2, '0');
            string msg = "{\"username\": \"" + this.username + "\"}";
            data += msg.Length.ToString().PadLeft(4, '0');
            data += msg;
            SendData(data, this.sock);
        }

        public void SendData(string data, Socket sock)
        {
            byte[] msg = new byte[data.Length];
            for (int i = 0; i < data.Length; i++)
                msg[i] = (byte)data[i];
            sock.Send(msg);
        }

        public ServerMsg GetData()
        {
            ServerMsg msg;
            byte[] code = new byte[2];
            this.sock.Receive(code, 2, 0);
            msg.code = Encoding.ASCII.GetString(code);
            byte[] len = new byte[4];
            this.sock.Receive(len, 4, 0);
            int size = Int32.Parse(Encoding.ASCII.GetString(len, 0, 4));
            byte[] data = new byte[size];
            this.sock.Receive(data, size, 0);
            msg.data = Encoding.ASCII.GetString(data);

            return msg;
        }
    }

    public struct ServerMsg
    {
        public string code;
        public string data;
    }
}
