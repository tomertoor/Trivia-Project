using System;
using System.Net.Sockets;
using System.Text;
using System.Security.Cryptography;
using System.Linq;

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
        public const string IP = "85.250.126.121";
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
        public const string GET_ROOM_STATE = "13";
        public const string HIGH_SCORES = "10";
        public const string CLOSE_ROOM = "11";
        public const string START_GAME = "12";
        public const string LEAVE_ROOM = "14";
        public const string GET_QUESTION = "16";
        public const string LEAVE_GAME = "15";
        public const string SUBMIT_ANSWER = "17";
        public const string GET_RESULTS = "18";
        public const string ADMIN = "admin";
        public const string MEMBER = "member";
        public const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
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
            PAZCryptoAlgorithm algorithm = new PAZCryptoAlgorithm(""); //encryption does not require a key, generates a random one
            var msg = algorithm.Encrypt(data);
            var key = Encoding.ASCII.GetBytes(algorithm.GetKey());
            byte[] sen = new byte[msg.Length + key.Length];
            System.Buffer.BlockCopy(key, 0, sen, 0, key.Length);
            System.Buffer.BlockCopy(msg, 0, sen, key.Length, msg.Length);
            sock.Send(sen);
        }

        public ServerMsg GetData()
        {
            byte[] key = new byte[16];
            this.sock.Receive(key, 16, 0);
            PAZCryptoAlgorithm algorithm = new PAZCryptoAlgorithm(Encoding.ASCII.GetString(key));
            ServerMsg msg;
            byte[] code = new byte[2];
            this.sock.Receive(code, 2, 0);
            msg.code = algorithm.Decrypt(code);
            byte[] len = new byte[4];
            this.sock.Receive(len, 4, 0);
            int size = Int32.Parse(algorithm.Decrypt(len));
            byte[] data = new byte[size];
            this.sock.Receive(data, size, 0);
            msg.data = algorithm.Decrypt(data);

            return msg;
        }
    }

    public struct ServerMsg
    {
        public string code;
        public string data;
    }

    public abstract class CryptoAlgorithm
    {
        public abstract byte[] Encrypt(string msg);
        public abstract string Decrypt(byte[] msg);
    }

    public class PAZCryptoAlgorithm : CryptoAlgorithm
    {
        private string key;
        private int lastIdxKey;
        public PAZCryptoAlgorithm(string Key) { key = Key; lastIdxKey = 0; }
        public override byte[] Encrypt(string msg)
        {
            lastIdxKey = 0;
            byte[] result = new byte[msg.Length];
            var random = new Random();
            key = new string(System.Linq.Enumerable.Repeat(Consts.chars, 16).Select(s => s[random.Next(s.Length)]).ToArray());
            for(int i = 0; i<msg.Length; i++)
            {
                var sum = ((int)msg[i] + (int)key[i % key.Length]) % 255;
                result[i] = (byte)sum;
            }
            var s = Encoding.ASCII.GetString(result);
            return result;
        }
        public override string Decrypt(byte[] msg)
        {
            string result = "";
            for (int i = 0; i < msg.Length; lastIdxKey++, i++)
            {
                var dif = ((int)msg[i] - (int)key[lastIdxKey % key.Length]) % 255;
                result += Convert.ToChar(dif);
            }
            return result;
        }

        public string GetKey() { return key; }
    }
}
