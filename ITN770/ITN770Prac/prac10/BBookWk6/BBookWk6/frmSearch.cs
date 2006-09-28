using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Globalization;

namespace BirthdayBook
{
    public partial class frmSearch : Form
    {
        private BBook bbook; 

        public frmSearch(BBook bb)
        {
            this.bbook = bb; 
            InitializeComponent();
        }

        private void frmSearch_Load(object sender, EventArgs e)
        {
            LoadCultures();
        }

        private void LoadCultures()
        {
            CultureInfo[] list;
            String name;
                        
            list = CultureInfo.GetCultures(CultureTypes.InstalledWin32Cultures);
            
            //Fill the combo boxes
            cbxCulture.Items.Clear();

            //There is a name and a display name
            foreach (CultureInfo ci in list)
            {
                name = ci.EnglishName;
                
                //bind the names together and show for 
                //both culture and UIculture
                name = ci.Name + " " + name;

                cbxCulture.Items.Add(name);
            }

            cbxCulture.SelectedIndex = 0;

        }

        private void btnUpdate_Click(object sender, EventArgs e)
        {
            string key = txtInput.Text;
            //this.bbook.BBookTable.

            foreach (DataRow row in bbook.BBookTable.Rows)
            {
                string fName = row[0].ToString();
                string item = "";
                MessageBox.Show(fName);
                if (key == fName)
                {
                    foreach (DataColumn column in bbook.BBookTable.Columns)
                    {
                        item += row[column]+",\t";
                    }

                    lbxResult.Items.Add(item);
                }
            }
        }
    }
}