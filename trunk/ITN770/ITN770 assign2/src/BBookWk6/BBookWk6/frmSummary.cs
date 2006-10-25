using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace BirthdayBook
{
    public partial class frmSummary : Form
    {
        private BBook bbook;

        public frmSummary(BBook bb)
        {
            this.bbook = bb;
            InitializeComponent();
        }

        private void LoadUIStrings()
        {
            StringResManager res = new StringResManager("BirthdayBook.UIStrings", this.GetType().Assembly);
            //Form 
            this.Text = res.GetString("[SUMMARY]");

            //Buttons
            this.btnClose.Text = res.GetString("[CLOSE]");

            //labels
            this.lblColumnNameText.Text = res.GetString("[COLUMN NAME]");
            this.lblColumnNumText.Text = res.GetString("[COLUMN NUMBER]");
            this.lblRowNumberText.Text = res.GetString("[ROW NUMBER]");

            //groupbox
            this.gbxInfo.Text = res.GetString("[INFORMATION]");

            //Right to left mirroring
            if (res.GetString("RTL") == "Yes")
            {
                this.RightToLeft = RightToLeft.Yes;
            }
            else
            {
                this.RightToLeft = RightToLeft.No;
            }
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void frmSummary_Load(object sender, EventArgs e)
        {
            this.lblColumnNumberValue.Text = Convert.ToString(this.bbook.BBookTable.Columns.Count);
            foreach (DataColumn column in bbook.BBookTable.Columns)
            {
                this.lblColumnNameValue.Text += column.ColumnName + ", "; 
            }
            this.lblRowNumberValue.Text = Convert.ToString(this.bbook.BBookTable.Rows.Count);
            LoadUIStrings();
        }
    }
}