#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: gps_rft_v1
# GNU Radio version: 3.10.7.0

from packaging.version import Version as StrictVersion
from PyQt5 import Qt
from gnuradio import qtgui
from gnuradio import blocks
import pmt
from gnuradio import gnss
from gnuradio import gr
from gnuradio.filter import firdes
from gnuradio.fft import window
import sys
import signal
from PyQt5 import Qt
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
import sip



class gps_rft_v1(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "gps_rft_v1", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("gps_rft_v1")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except BaseException as exc:
            print(f"Qt GUI: Could not set Icon: {str(exc)}", file=sys.stderr)
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "gps_rft_v1")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except BaseException as exc:
            print(f"Qt GUI: Could not restore geometry: {str(exc)}", file=sys.stderr)

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 4000000
        self.pll_bw_fine = pll_bw_fine = 35.0
        self.pll_bw_coarse = pll_bw_coarse = 40.0
        self.dll_bw_fine = dll_bw_fine = 2.0
        self.dll_bw_coarse = dll_bw_coarse = 4.0

        ##################################################
        # Blocks
        ##################################################

        self.qtgui_time_sink_x_0_0_0_0_0 = qtgui.time_sink_f(
            200, #size
            1000, #samp_rate
            "", #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0_0_0_0_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0_0_0_0_0.set_y_axis(1023000-20, 1023000+20)

        self.qtgui_time_sink_x_0_0_0_0_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0_0_0_0_0.enable_tags(False)
        self.qtgui_time_sink_x_0_0_0_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_0_0_0_0.enable_autoscale(True)
        self.qtgui_time_sink_x_0_0_0_0_0.enable_grid(False)
        self.qtgui_time_sink_x_0_0_0_0_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0_0_0_0_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0_0_0_0_0.enable_stem_plot(False)


        labels = ['Signal 1', 'Signal 2', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [-1, 0, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0_0_0_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0_0_0_0_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_0_0_0_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_0_0_0_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_0_0_0_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_0_0_0_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_0_0_0_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_0_0_0_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_0_0_0_0.qwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_0_0_0_0_win)
        self.qtgui_time_sink_x_0_0_0_0 = qtgui.time_sink_c(
            200, #size
            1000, #samp_rate
            "", #name
            1, #number of inputs
            None # parent
        )
        self.qtgui_time_sink_x_0_0_0_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0_0_0_0.set_y_axis(0, 4000)

        self.qtgui_time_sink_x_0_0_0_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0_0_0_0.enable_tags(False)
        self.qtgui_time_sink_x_0_0_0_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0_0_0_0.enable_autoscale(True)
        self.qtgui_time_sink_x_0_0_0_0.enable_grid(False)
        self.qtgui_time_sink_x_0_0_0_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0_0_0_0.enable_control_panel(False)
        self.qtgui_time_sink_x_0_0_0_0.enable_stem_plot(False)


        labels = ['Signal 1', 'Signal 2', 'Signal 3', 'Signal 4', 'Signal 5',
            'Signal 6', 'Signal 7', 'Signal 8', 'Signal 9', 'Signal 10']
        widths = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        colors = ['blue', 'red', 'green', 'black', 'cyan',
            'magenta', 'yellow', 'dark red', 'dark green', 'dark blue']
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, 1.0, 1.0]
        styles = [1, 1, 1, 1, 1,
            1, 1, 1, 1, 1]
        markers = [0, 0, -1, -1, -1,
            -1, -1, -1, -1, -1]


        for i in range(2):
            if len(labels[i]) == 0:
                if (i % 2 == 0):
                    self.qtgui_time_sink_x_0_0_0_0.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_0_0_0_0.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_0_0_0_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_0_0_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_0_0_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_0_0_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_0_0_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_0_0_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_0_0_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_0_0_0.qwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_0_0_0_win)
        self.gnss_type_converter_0_0_0 = gnss.f64_to_f32(1)
        self.gnss_type_converter_0_0 = gnss.fc64_to_fc32(1)
        self.gnss_signal_normalizer_0 = gnss.signal_normalizer_s16_fc32(1, 12)
        self.gnss_rft_0 = gnss.rft()
        self.gnss_rft_0.set_transformation(gnss.RFT_ECEF_TO_GCS)
        self.gnss_pvt_user_0 = gnss.pvt_user(True)
        self.gnss_pseudoranges_decoder_0 = gnss.pseudoranges_decoder(False)
        self.gnss_pseudoranges_decoder_0.set_acq_params(0, gnss.NAVIGATION_SYSTEM_GPS, 1)
        self.gnss_pseudoranges_decoder_0.set_acq_params(1, gnss.NAVIGATION_SYSTEM_GPS, 11)
        self.gnss_pseudoranges_decoder_0.set_acq_params(2, gnss.NAVIGATION_SYSTEM_GPS, 20)
        self.gnss_pseudoranges_decoder_0.set_acq_params(3, gnss.NAVIGATION_SYSTEM_GPS, 32)
        self.gnss_pseudoranges_decoder_0.set_acq_params(4, gnss.NAVIGATION_SYSTEM_UNDEFINED, (-1))
        self.gnss_pseudoranges_decoder_0.set_acq_params(5, gnss.NAVIGATION_SYSTEM_UNDEFINED, (-1))
        self.gnss_pseudoranges_decoder_0.set_acq_params(6, gnss.NAVIGATION_SYSTEM_UNDEFINED, (-1))
        self.gnss_pseudoranges_decoder_0.set_acq_params(7, gnss.NAVIGATION_SYSTEM_UNDEFINED, (-1))
        self.gnss_nav_message_decoder_0_0_0_0 = gnss.nav_message_decoder()
        self.gnss_nav_message_decoder_0_0_0_0.set_acq_params(gnss.NAVIGATION_SYSTEM_GPS, 32)
        self.gnss_nav_message_decoder_0_0_0 = gnss.nav_message_decoder()
        self.gnss_nav_message_decoder_0_0_0.set_acq_params(gnss.NAVIGATION_SYSTEM_GPS, 20)
        self.gnss_nav_message_decoder_0_0 = gnss.nav_message_decoder()
        self.gnss_nav_message_decoder_0_0.set_acq_params(gnss.NAVIGATION_SYSTEM_GPS, 11)
        self.gnss_nav_message_decoder_0 = gnss.nav_message_decoder()
        self.gnss_nav_message_decoder_0.set_acq_params(gnss.NAVIGATION_SYSTEM_GPS, 1)
        self.gnss_geojson_file_sink_1 = gnss.geojson_file_sink("default")
        self.gnss_ca_symbols_to_nav_bits_0_2 = gnss.ca_symbols_to_nav_bits()
        self.gnss_ca_symbols_to_nav_bits_0_1 = gnss.ca_symbols_to_nav_bits()
        self.gnss_ca_symbols_to_nav_bits_0_0 = gnss.ca_symbols_to_nav_bits()
        self.gnss_ca_symbols_to_nav_bits_0 = gnss.ca_symbols_to_nav_bits()
        self.gnss_acquisition_and_tracking_0_0_0_0 = gnss.acquisition_and_tracking(samp_rate, dll_bw_coarse, pll_bw_coarse, dll_bw_fine, pll_bw_fine)
        self.gnss_acquisition_and_tracking_0_0_0_0.set_acq_params(gnss.NAVIGATION_SYSTEM_GPS, 32)
        self.gnss_acquisition_and_tracking_0_0_0 = gnss.acquisition_and_tracking(samp_rate, dll_bw_coarse, pll_bw_coarse, dll_bw_fine, pll_bw_fine)
        self.gnss_acquisition_and_tracking_0_0_0.set_acq_params(gnss.NAVIGATION_SYSTEM_GPS, 20)
        self.gnss_acquisition_and_tracking_0_0 = gnss.acquisition_and_tracking(samp_rate, dll_bw_coarse, pll_bw_coarse, dll_bw_fine, pll_bw_fine)
        self.gnss_acquisition_and_tracking_0_0.set_acq_params(gnss.NAVIGATION_SYSTEM_GPS, 11)
        self.gnss_acquisition_and_tracking_0 = gnss.acquisition_and_tracking(samp_rate, dll_bw_coarse, pll_bw_coarse, dll_bw_fine, pll_bw_fine)
        self.gnss_acquisition_and_tracking_0.set_acq_params(gnss.NAVIGATION_SYSTEM_GPS, 1)
        self.blocks_throttle2_0 = blocks.throttle( gr.sizeof_gr_complex*1, samp_rate, True, 0 if "auto" == "auto" else max( int(float(0.1) * samp_rate) if "auto" == "time" else int(0.1), 1) )
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_short*1, '/home/memyselfandi/projects/sdr-files/gnss/2013_04_04_GNSS_SIGNAL_at_CTTC_SPAIN/2013_04_04_GNSS_SIGNAL_at_CTTC_SPAIN.dat', False, 0, 0)
        self.blocks_file_source_0.set_begin_tag(pmt.PMT_NIL)


        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.gnss_nav_message_decoder_0, 'clock'), (self.gnss_pseudoranges_decoder_0, 'clock'))
        self.msg_connect((self.gnss_nav_message_decoder_0, 'ephemeris'), (self.gnss_pseudoranges_decoder_0, 'ephemeris'))
        self.msg_connect((self.gnss_nav_message_decoder_0_0, 'ephemeris'), (self.gnss_pseudoranges_decoder_0, 'ephemeris'))
        self.msg_connect((self.gnss_nav_message_decoder_0_0, 'clock'), (self.gnss_pseudoranges_decoder_0, 'clock'))
        self.msg_connect((self.gnss_nav_message_decoder_0_0_0, 'clock'), (self.gnss_pseudoranges_decoder_0, 'clock'))
        self.msg_connect((self.gnss_nav_message_decoder_0_0_0, 'ephemeris'), (self.gnss_pseudoranges_decoder_0, 'ephemeris'))
        self.msg_connect((self.gnss_nav_message_decoder_0_0_0_0, 'clock'), (self.gnss_pseudoranges_decoder_0, 'clock'))
        self.msg_connect((self.gnss_nav_message_decoder_0_0_0_0, 'ephemeris'), (self.gnss_pseudoranges_decoder_0, 'ephemeris'))
        self.connect((self.blocks_file_source_0, 0), (self.gnss_signal_normalizer_0, 0))
        self.connect((self.blocks_throttle2_0, 0), (self.gnss_acquisition_and_tracking_0, 0))
        self.connect((self.blocks_throttle2_0, 0), (self.gnss_acquisition_and_tracking_0_0, 0))
        self.connect((self.blocks_throttle2_0, 0), (self.gnss_acquisition_and_tracking_0_0_0, 0))
        self.connect((self.blocks_throttle2_0, 0), (self.gnss_acquisition_and_tracking_0_0_0_0, 0))
        self.connect((self.gnss_acquisition_and_tracking_0, 0), (self.gnss_ca_symbols_to_nav_bits_0, 0))
        self.connect((self.gnss_acquisition_and_tracking_0, 0), (self.gnss_type_converter_0_0, 0))
        self.connect((self.gnss_acquisition_and_tracking_0, 1), (self.gnss_type_converter_0_0_0, 0))
        self.connect((self.gnss_acquisition_and_tracking_0_0, 0), (self.gnss_ca_symbols_to_nav_bits_0_0, 0))
        self.connect((self.gnss_acquisition_and_tracking_0_0_0, 0), (self.gnss_ca_symbols_to_nav_bits_0_1, 0))
        self.connect((self.gnss_acquisition_and_tracking_0_0_0_0, 0), (self.gnss_ca_symbols_to_nav_bits_0_2, 0))
        self.connect((self.gnss_ca_symbols_to_nav_bits_0, 0), (self.gnss_nav_message_decoder_0, 0))
        self.connect((self.gnss_ca_symbols_to_nav_bits_0_0, 0), (self.gnss_nav_message_decoder_0_0, 0))
        self.connect((self.gnss_ca_symbols_to_nav_bits_0_1, 0), (self.gnss_nav_message_decoder_0_0_0, 0))
        self.connect((self.gnss_ca_symbols_to_nav_bits_0_2, 0), (self.gnss_nav_message_decoder_0_0_0_0, 0))
        self.connect((self.gnss_nav_message_decoder_0, 0), (self.gnss_pseudoranges_decoder_0, 0))
        self.connect((self.gnss_nav_message_decoder_0_0, 0), (self.gnss_pseudoranges_decoder_0, 1))
        self.connect((self.gnss_nav_message_decoder_0_0_0, 0), (self.gnss_pseudoranges_decoder_0, 2))
        self.connect((self.gnss_nav_message_decoder_0_0_0_0, 0), (self.gnss_pseudoranges_decoder_0, 3))
        self.connect((self.gnss_pseudoranges_decoder_0, 2), (self.gnss_pvt_user_0, 2))
        self.connect((self.gnss_pseudoranges_decoder_0, 0), (self.gnss_pvt_user_0, 0))
        self.connect((self.gnss_pseudoranges_decoder_0, 3), (self.gnss_pvt_user_0, 3))
        self.connect((self.gnss_pseudoranges_decoder_0, 1), (self.gnss_pvt_user_0, 1))
        self.connect((self.gnss_pvt_user_0, 0), (self.gnss_rft_0, 0))
        self.connect((self.gnss_rft_0, 0), (self.gnss_geojson_file_sink_1, 0))
        self.connect((self.gnss_signal_normalizer_0, 0), (self.blocks_throttle2_0, 0))
        self.connect((self.gnss_type_converter_0_0, 0), (self.qtgui_time_sink_x_0_0_0_0, 0))
        self.connect((self.gnss_type_converter_0_0_0, 0), (self.qtgui_time_sink_x_0_0_0_0_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "gps_rft_v1")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle2_0.set_sample_rate(self.samp_rate)

    def get_pll_bw_fine(self):
        return self.pll_bw_fine

    def set_pll_bw_fine(self, pll_bw_fine):
        self.pll_bw_fine = pll_bw_fine

    def get_pll_bw_coarse(self):
        return self.pll_bw_coarse

    def set_pll_bw_coarse(self, pll_bw_coarse):
        self.pll_bw_coarse = pll_bw_coarse

    def get_dll_bw_fine(self):
        return self.dll_bw_fine

    def set_dll_bw_fine(self, dll_bw_fine):
        self.dll_bw_fine = dll_bw_fine

    def get_dll_bw_coarse(self):
        return self.dll_bw_coarse

    def set_dll_bw_coarse(self, dll_bw_coarse):
        self.dll_bw_coarse = dll_bw_coarse




def main(top_block_cls=gps_rft_v1, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    qapp.exec_()

if __name__ == '__main__':
    main()
