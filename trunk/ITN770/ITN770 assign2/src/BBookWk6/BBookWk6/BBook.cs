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
        const string NameCol = "[NAME]";
        const string DateCol = "[BIRTHDAY]";
        const int GridLineWidth = 1;
        const int NumColumns = 2;               // Number of columns
        const string Separator = ";";           // Field separator

        private string strTextFiles = "[Text Files]";
        private string strAllFiles = "[All Files]";
        private string strFatalError = "[Fatal Error]";
        private Boolean RTL = false;
        private string messageboxButton = "";
        private string messageCaption = "";
        private string openDialogTitle = "";
        private string saveDialogTitle = "";

        private Boolean firstTime = true;      //check whether the form is loaded for the fist time

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
            this.cmdViewXML.Text = res.GetString("[XML]");
            this.btnSummary.Text = res.GetString("[SUMMARY]");

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

            //Right to left mirroring
            if (res.GetString("RTL") == "Yes")
            {
                this.RightToLeft = RightToLeft.Yes;
                this.RTL = true;
            }
            else
            {
                this.RightToLeft = RightToLeft.No;
            }

            //messagebox button
            this.messageboxButton = res.GetString("[OK]");
            this.messageCaption = res.GetString("[DATASET]");

            //open and close file dialog
            this.openDialogTitle = res.GetString("[OPEN]");
            this.saveDialogTitle = res.GetString("[SAVE AS]");
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
            this.m_BBookTable.Columns.Add(new DataColumn(NameCol, Type.GetType("System.String")));
            this.m_BBookTable.Columns.Add(new DataColumn(DateCol, Type.GetType("System.DateTime")));

            //The GridView needs a source, and the table should be added to the set
            BBookGrid.DataSource = m_BBookTable;
            m_ds.Tables.Add(m_BBookTable);

            //Set up a better looking table 
            BBookGrid.Columns[NameCol].Width = (BBookGrid.Width - BBookGrid.RowHeadersWidth - 2 * GridLineWidth) / NumColumns 
                                                - GridLineWidth;
            BBookGrid.Columns[DateCol].Width = BBookGrid.Columns[NameCol].Width;

            if (firstTime)
            {
                Thread.CurrentThread.CurrentUICulture = Thread.CurrentThread.CurrentCulture;
                firstTime = false;
            }
            
            LoadUIStrings();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close(); 
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenDialog.Reset();
            OpenDialog.InitialDirectory = Directory.GetCurrentDirectory();
            OpenDialog.Title = this.openDialogTitle;
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
            SaveDialog.Title = this.saveDialogTitle;
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
            if (this.RTL == true)
            {
                MessageBoxOptions option = (MessageBoxOptions)0;
                option = MessageBoxOptions.RtlReading |
               MessageBoxOptions.RightAlign;

                MessageBoxEx.Show(m_ds.GetXml().ToString(), this.messageCaption, MessageBoxIcon.Question, MessageBoxButtons.OK,option, this.messageboxButton);
            }
            else
            {
                //MessageBox.Show(m_ds.GetXml().ToString(),"DataSet",btnOk);
                MessageBoxEx.Show(m_ds.GetXml().ToString(), this.messageCaption, MessageBoxIcon.Question, MessageBoxButtons.OK, this.messageboxButton);
            }
        }

        private void btnDetails_Click(object sender, EventArgs e)
        {
            frmSummary summary = new frmSummary(this);
            summary.ShowDialog();
        }

        public DataTable BBookTable
        {
            get { return this.m_BBookTable; }
        }
    }
}