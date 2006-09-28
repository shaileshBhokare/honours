using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO; 
using System.Resources;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace BirthdayBook
{
    public partial class BBook : Form
    {
        const string DataSetName = "BBData";
        const string TableName = "Birthdays";

        //const string NameCol = "[NAME]";
        const string FamilyCol = "[FAMILY]";
        const string GivenCol = "[GIVEN]";
        const string TitleCol = "[TITLE]";


        const string DateCol = "[BIRTHDAY]";
        const int GridLineWidth = 1;
        const int NumColumns = 2;               // Number of columns
        const string Separator = ";";           // Field separator

        private string strTextFiles = "[Text Files]";
        private string strAllFiles = "[All Files]";
        private string strFatalError = "[Fatal Error]";
        private string strNoRowError = "[NoRowError]";
        private string strGreeting = "[Greeting]";

        private DataSet m_ds;
        private DataTable m_BBookTable;	        //stores the birthdays 

        public BBook()
        {
            InitializeComponent();
        }

        private void LoadUIStrings() {
            StringResManager res = new StringResManager("BirthdayBook.UIStrings", this.GetType().Assembly);
            //Form 
            this.Text = res.GetString("[BBOOK]");

            //Buttons
            this.cmdExit.Text = res.GetString("[EXIT]");
            this.cmdSelect.Text = res.GetString("[SETCULTURE]");
            this.cmdSelect.Text = res.GetString("[Bogus]");

            //Menus
            this.fileToolStripMenuItem.Text = res.GetString("[FILE]");
            this.openToolStripMenuItem.Text = res.GetString("[OPEN]");
            this.saveToolStripMenuItem.Text = res.GetString("[SAVE]");
            this.exitToolStripMenuItem.Text = res.GetString("[EXIT]");

            //DataGridView
            this.m_BBookTable.Columns[0].ColumnName = res.GetString("[NAME]");
            this.m_BBookTable.Columns[1].ColumnName = res.GetString("[BIRTHDAY]");

            //Messages
            this.strAllFiles = res.GetString("[ALLFILES]");
            this.strTextFiles = res.GetString("[TEXTFILES]");
            this.strFatalError = res.GetString("[FATALERROR]");
            this.strGreeting = res.GetString("[NOROWERROR]");
            if(res.GetString("RTL")=="Yes"){
                this.RightToLeft= RightToLeft.Yes;
            }else{
                this.RightToLeft=RightToLeft.No;
            }

        }

        private void cmdExit_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void BBook_Load(object sender, EventArgs e)
        {
            //Initialize everything on startup
            //Create a data table with two columns
            this.m_ds = new DataSet(DataSetName);
            this.m_BBookTable = new DataTable(TableName);
            //this.m_BBookTable.Columns.Add(new DataColumn(NameCol, Type.GetType("System.String")));
            this.m_BBookTable.Columns.Add(new DataColumn(FamilyCol, Type.GetType("System.String")));
            this.m_BBookTable.Columns.Add(new DataColumn(GivenCol, Type.GetType("System.String")));
            this.m_BBookTable.Columns.Add(new DataColumn(TitleCol, Type.GetType("System.String")));
            this.m_BBookTable.Columns.Add(new DataColumn(DateCol, Type.GetType("System.DateTime")));

            //The GridView needs a source, and the table should be added to the set
            BBookGrid.DataSource = m_BBookTable;
            m_ds.Tables.Add(m_BBookTable);

            //Set up a better looking table 
            //BBookGrid.Columns[NameCol].Width = (BBookGrid.Width - BBookGrid.RowHeadersWidth - 2 * GridLineWidth) / NumColumns  - GridLineWidth;
            BBookGrid.Columns[FamilyCol].Width = (BBookGrid.Width - BBookGrid.RowHeadersWidth - 2 * GridLineWidth) / NumColumns - GridLineWidth;
            BBookGrid.Columns[GivenCol].Width = BBookGrid.Columns[FamilyCol].Width;
            BBookGrid.Columns[TitleCol].Width = BBookGrid.Columns[FamilyCol].Width; 
            BBookGrid.Columns[DateCol].Width = BBookGrid.Columns[FamilyCol].Width; 
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close(); 
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenDialog.Reset();
            OpenDialog.InitialDirectory = Directory.GetCurrentDirectory();
            OpenDialog.RestoreDirectory = false;
            OpenDialog.Filter = String.Format("{0} (*.txt)|*.txt|{1} (*.*)|*.*", this.strTextFiles,
                                                this.strAllFiles);
            if (OpenDialog.ShowDialog() == DialogResult.OK) {  
                try {
                    StreamReader MyStream = new StreamReader(OpenDialog.FileName);
                    m_BBookTable.Clear();
                    m_ds.ReadXml(MyStream);
                    BBookGrid.DataSource = m_ds.Tables[TableName];
                    MyStream.Close();
                } catch (Exception ex) {
                    MessageBox.Show(this.strFatalError+" "+ex.ToString());
                    Application.Exit(); 
                } 
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveDialog.Reset();
            SaveDialog.InitialDirectory = Directory.GetCurrentDirectory();
            SaveDialog.RestoreDirectory = false;
            SaveDialog.Filter = String.Format("{0} (*.txt)|*.txt|{1} (*.*)|*.*", this.strTextFiles,
                                                this.strAllFiles);

            if (SaveDialog.ShowDialog() == DialogResult.OK) {
                try {
                    StreamWriter MyStream = new StreamWriter(SaveDialog.FileName);
                    this.m_ds.WriteXml(MyStream);
                    MyStream.Close();
                } catch (Exception ex) {
                    MessageBox.Show(this.strFatalError + " " + ex.ToString());
                    Application.Exit();
                }
            }
        }

        private void cmdSelect_Click(object sender, EventArgs e) {
             SelectCulture s = new SelectCulture();
             if (s.ShowDialog() == DialogResult.OK) { 
                LoadUIStrings();
             }
        }

        private void cmdViewXML_Click(object sender, EventArgs e) {
            MessageBox.Show(m_ds.GetXml().ToString());
        }

        private void cbxRTL_CheckedChanged(object sender, EventArgs e)
        {
            if (this.cbxRTL.Checked)
            {
                this.RightToLeft = RightToLeft.Yes;
            }
            else
            {
                this.RightToLeft = RightToLeft.No;
            }
        }

        private void btnGreeting_Click(object sender, EventArgs e)
        {
            string str;
            if (this.BBookGrid.CurrentRow != null)
            {
                //str = "Row Available";
                //MessageBox.Show(str, "BirthdayBook", MessageBoxButtons.OK, MessageBoxIcon.Information);

                int index = this.BBookGrid.NewRowIndex - 1;
                DataRow Row = m_ds.Tables[TableName].Rows[index];
                str = Row[TitleCol].ToString();
                MessageBox.Show(str, "BirthdayBook", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            else
            {
                MessageBox.Show(strNoRowError, "BirthdayBook", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        public DataTable BBookTable
        {
            get { return this.m_BBookTable; }
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            frmSearch search = new frmSearch(this);
            search.ShowDialog();
        }
    }
}