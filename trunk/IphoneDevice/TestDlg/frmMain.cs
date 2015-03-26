using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using IphoneReadLib;
namespace iTunesDLLDebug
{
    public partial class frmMain : Form
    {
        
        public frmMain()
        {
            InitializeComponent();
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            
            
        }
        /*
                    Valid Value Names:
                            ActivationState
                            ActivationStateAcknowledged
                            BasebandBootloaderVersion
                            BasebandVersion
                            BluetoothAddress
                            BuildVersion
                            DeviceCertificate
                            DeviceClass
                            DeviceName
                            DevicePublicKey
                            FirmwareVersion
                            HostAttached
                            IntegratedCircuitCardIdentity
                            InternationalMobileEquipmentIdentity
                            InternationalMobileSubscriberIdentity
                            ModelNumber
                            PhoneNumber
                            ProductType
                            ProductVersion
                            ProtocolVersion
                            RegionInfo
                            SBLockdownEverRegisteredKey
                            SIMStatus
                            SerialNumber
                            SomebodySetTimeZone
                            TimeIntervalSince1970
                            TimeZone
                            TimeZoneOffsetFromUTC
                            TrustedHostAttached
                            UniqueDeviceID
                            Uses24HourClock
                            WiFiAddress
                            iTunesHasConnected
                 */


        private void button1_Click(object sender, EventArgs e)
        {
            DeviceInfo device = CIphoneRead.GetDeviceInfo();
            if (!device.IsConnected)
            {
                MessageBox.Show("δ����");
                return;
            }
            string strShow = string.Format("�豸���ƣ�{0}\r\n�豸��ʶ��{1}\r\n��Ʒ���ͣ�{2}\r\nϵͳ�汾��{3}\r\n���кţ�{4}\r\nIMEI��{5}\r\n����״̬��{6}",
                device.DeviceName, device.DeviceId, device.DeviceType, device.DeviceVersion,
                device.DeviceSerial,
                device.IMEI,
                device.ActivationState);
            strShow = strShow.Replace('\0', ' ');
            MessageBox.Show(strShow);
        }
    }
}