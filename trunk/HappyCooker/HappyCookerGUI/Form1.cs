using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.IO;

namespace HappyCookerGUI
{
    enum CookerState
    {
        File, Action
    }
    public partial class Form1 : Form
    {
        Process p;
        CookerState state;
        StreamWriter pIn;

        OpenFileDialog m_OpenFileDialog;

        SaveFileDialog m_SaveFileDialog;

        public Form1()
        {
            InitializeComponent();

            p = new Process();
            p.StartInfo.FileName = "HappyCooker.exe";
            p.StartInfo.CreateNoWindow = true;
            p.StartInfo.RedirectStandardInput = true;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.RedirectStandardError = true;
            p.StartInfo.UseShellExecute = false;
            p.Start();

            pIn = p.StandardInput;

            state = CookerState.File;

            m_OpenFileDialog = new OpenFileDialog();
            m_OpenFileDialog.AddExtension = true;
            m_OpenFileDialog.AutoUpgradeEnabled = true;
            m_OpenFileDialog.CheckFileExists = true;
            m_OpenFileDialog.CheckPathExists = true;
            m_OpenFileDialog.DefaultExt = ".obj";
            m_OpenFileDialog.Filter = "*.obj|*.obj";
            m_OpenFileDialog.InitialDirectory = Environment.CurrentDirectory;
            m_OpenFileDialog.Multiselect = false;
            m_OpenFileDialog.RestoreDirectory = true;
            m_OpenFileDialog.Title = "Import Obj";

            m_SaveFileDialog = new SaveFileDialog();
            m_SaveFileDialog.AddExtension = true;
            m_SaveFileDialog.AutoUpgradeEnabled = true;
            m_SaveFileDialog.CheckPathExists = true;
            m_SaveFileDialog.DefaultExt = ".bin";
            m_SaveFileDialog.Filter = "*.bin|*.bin";
            m_SaveFileDialog.InitialDirectory = Environment.CurrentDirectory;
            m_SaveFileDialog.RestoreDirectory = true;
            m_SaveFileDialog.Title = "Export Obj";
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            p.Kill();
        }

        private void btn_LoadObj_Click(object sender, EventArgs e)
        {
            if (state == CookerState.Action)
            {
                pIn.WriteLine("-1");
                state = CookerState.File;
            }
            if (m_OpenFileDialog.ShowDialog() != System.Windows.Forms.DialogResult.Cancel)
            {
                pB_LoadObj.Value = 0;

                pIn.WriteLine(m_OpenFileDialog.FileName);
                
                pB_LoadObj.Value = 100;
            }
        }

        private string GetSaveName(string title, string extension)
        {
            m_SaveFileDialog.Title = title;
            m_SaveFileDialog.DefaultExt = extension;
            m_SaveFileDialog.Filter = "*" + extension + "|*" + extension;

            if (m_SaveFileDialog.ShowDialog() != System.Windows.Forms.DialogResult.Cancel)
            {
                return m_SaveFileDialog.FileName;
            }
            else
                return "";
        }

        private void btn_CookBin_Click(object sender, EventArgs e)
        {
            string name = GetSaveName("Save to binary obj", ".bobj");

            pB_LoadObj.Value = 0;
            if (name != "")
            {
                pIn.WriteLine("0");
                pIn.WriteLine(name);
            }
            pB_LoadObj.Value = 100;
        }

        private void btn_CookConvex_Click(object sender, EventArgs e)
        {
            string name = GetSaveName("Save convex physX mesh", ".cxpm");

            pB_LoadObj.Value = 0;
            if (name != "")
            {
                pIn.WriteLine("1");
                pIn.WriteLine(name);
            }
            pB_LoadObj.Value = 100;
        }

        private void btn_CookConcave_Click(object sender, EventArgs e)
        {
            string name = GetSaveName("Save concave physX mesh", ".ccpm");

            pB_LoadObj.Value = 0;
            if (name != "")
            {
                pIn.WriteLine("2");
                pIn.WriteLine(name);
            }
            pB_LoadObj.Value = 100;
        }
    }
}
