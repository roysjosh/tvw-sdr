Name:		tvw-sdr
Version:	2013.02.14
Release:	1%{?dist}
Summary:	Linux userspace driver for TVW750 (TVW750USB/TVW750U) devices

Group:		Development/Libraries
License:	GPLv2+
URL:		https://github.com/roysjosh/tvw-sdr
# git archive --prefix=tvw-sdr/ HEAD | gzip > tvw-sdr.tgz
Source0:	tvw-sdr.tgz

BuildRequires:	libusb1-devel
Requires:	libusb1

%description
Linux userspace driver for TVW750 (TVW750USB/TVW750U) devices


%prep
%setup -q -n %{name}


%build
autoreconf -i .
%configure
make %{?_smp_mflags}


%install
make install DESTDIR=%{buildroot}


%files
%doc AUTHORS COPYING README
%{_bindir}/tvw_sdr
%{_libdir}/libtvwsdr.la
%{_libdir}/libtvwsdr.so
%{_libdir}/libtvwsdr.so.0
%{_libdir}/libtvwsdr.so.0.0.0
%{_includedir}/%{name}.h


%changelog
* Thu Feb 14 2013 Joshua Roys <roysjosh@gmail.com> - 2013.02.14-1
- working library interface

* Thu Aug 16 2012 Joshua Roys <roysjosh@gmail.com> - 2012.08.16-1
- initial package
