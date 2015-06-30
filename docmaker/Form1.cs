using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using System.IO;

namespace docmaker
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() != DialogResult.OK) return;
            string fn = openFileDialog1.FileName;
            textBox1.Text = fn;
            if (File.Exists(fn))
            {
                XmlTextReader reader = null;
                try
                {
                    reader = new XmlTextReader(fn);
                    //ストリームからノードを読み取る
                    while (reader.Read())
                    {
                        if (reader.NodeType == XmlNodeType.Whitespace)
                        {
                            continue;
                        }

                        if (reader.NodeType == XmlNodeType.EndElement)
                        {
                            continue;
                        }

                        textBox2.Text += "NodeType = {" + reader.NodeType + "}, Name = {" + reader.Name + "}, Value = {" + reader.Value + "}\r\n";

                        if (reader.NodeType == XmlNodeType.Element)
                        {
                            //switch (reader.LocalName)
                            //{
                            //    case "param":
                            //        textBox2.Text += reader.ReadString() + "\r\n";
                            //        break;
                            //    case "example":
                            //        textBox2.Text += reader.ReadString() + "\r\n";
                            //        break;
                            //}
                            while (reader.MoveToNextAttribute())
                            {
                                Console.WriteLine("{0} = {1}", reader.Name, reader.Value);
                            }
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
                finally
                {
                    reader.Close();
                }
            }


        }
    }
}
