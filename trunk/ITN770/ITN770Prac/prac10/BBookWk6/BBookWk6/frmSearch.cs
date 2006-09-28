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

            lbxResult.Items.Clear();
            CultureInfo ci= new CultureInfo("en-US");
            int spaceIndex = 0;

            //We need to grab the string-based culture identifier
            String cultureID =  this.cbxCulture.Text;
            if (cultureID.Length!=0) { 
                spaceIndex = cultureID.IndexOf(" ");
                if (spaceIndex >= 0) {
                    cultureID = cultureID.Substring(0, spaceIndex);
                }

                //create new CultrueInfo objects
                ci = new CultureInfo(cultureID);
            }

            foreach (DataRow row in bbook.BBookTable.Rows)
            {
                string fName = row[0].ToString();
                string item = "";

                //MessageBox.Show(fName);
                if (key == fName)
                {
                    foreach (DataColumn column in bbook.BBookTable.Columns)
                    {
                        item += row[column]+",\t";
                    }
                    if (lbxResult.Items.Count != 0)
                    {
                        int index = 0;
                        Boolean condition = true;
                        while (condition)
                        {
                            try{
                                string existingItem = lbxResult.Items[index].ToString();
                            
                                int compare = String.Compare(item, existingItem, true, ci);
                                //MessageBox.Show(compare.ToString());
                                if (compare <= 0)
                                {
                                    lbxResult.Items.Insert(index, item);
                                    condition=false;
                                }
                                else
                                {
                                    index++;
                                }
                            }catch(ArgumentOutOfRangeException ex)
                            {
                                lbxResult.Items.Add(item);
                                //MessageBox.Show("only should show once");
                                condition = false;
                            }
                           
                        }
                    }
                    else
                    {
                        lbxResult.Items.Add(item);
                        //MessageBox.Show("only should show firstly");

                    }
                }
            }
        }

        private void btnSortKey_Click(object sender, EventArgs e)
        {
            string key = txtInput.Text;
            //this.bbook.BBookTable.

            lbxResult.Items.Clear();
            CultureInfo ci = new CultureInfo("en-US");
            int spaceIndex = 0;

            //We need to grab the string-based culture identifier
            String cultureID = this.cbxCulture.Text;
            if (cultureID.Length != 0)
            {
                spaceIndex = cultureID.IndexOf(" ");
                if (spaceIndex >= 0)
                {
                    cultureID = cultureID.Substring(0, spaceIndex);
                }

                //create new CultrueInfo objects
                ci = new CultureInfo(cultureID);
            }

            //create two sortkey
            SortKey key1, key2;
            key1 = ci.CompareInfo.GetSortKey(key);
            
            foreach (DataRow row in bbook.BBookTable.Rows)
            {
                string fName = row[0].ToString();
                string item = "";

                //MessageBox.Show(fName);
                if (key == fName)
                {
                    foreach (DataColumn column in bbook.BBookTable.Columns)
                    {
                        item += row[column] + ",\t";
                    }
                    if (lbxResult.Items.Count != 0)
                    {
                        int index = 0;
                        Boolean condition = true;
                        while (condition)
                        {
                            try
                            {
                                string existingItem = lbxResult.Items[index].ToString();

                                key2 = ci.CompareInfo.GetSortKey(existingItem);

                                int compare = SortKey.Compare(key1, key2);
                                //MessageBox.Show(compare.ToString());
                                if (compare <= 0)
                                {
                                    lbxResult.Items.Insert(index, item);
                                    condition = false;
                                }
                                else
                                {
                                    index++;
                                }
                            }
                            catch (ArgumentOutOfRangeException ex)
                            {
                                lbxResult.Items.Add(item);
                                //MessageBox.Show("only should show once");
                                condition = false;
                            }

                        }
                    }
                    else
                    {
                        lbxResult.Items.Add(item);
                        //MessageBox.Show("only should show firstly");

                    }
                }
            }
        }
    }
}