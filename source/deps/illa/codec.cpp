#include "StdAfx.h"

#include "codec.h"
#include "surface.h"

namespace Img {
	bool AbstractCodec::LoadHeader(IO::FileReader::Ptr reader) {
		try {
			if (reader == nullptr) {
				DO_THROW(Err::InvalidParam, TX("Reader was null."));
			}
			m_file = reader;

			{
				std::lock_guard<std::mutex> l(m_state);
				m_info.SurfaceFormat = Img::Format::Undefined;
				m_info.Dimensions = Geom::SizeInt(-1, -1);
				m_fs = reader->Size();
			}

			ImageInfo ii;
			ii.SurfaceFormat = Format::Undefined;

			if (PerformLoadHeader(reader, ii)) {
				// Validate info. Invalid values are treated as codec bugs.
				if (ii.SurfaceFormat == Img::Format::Undefined) {
					DO_THROW(Err::CodecError, TX("Surface format was set to invalid."));
				}
				if ((ii.Dimensions.Width <= 0) || (ii.Dimensions.Height <= 0)) {
					DO_THROW(Err::CodecError, TX("Dimensions were set to an invalid value."));
				}

				std::lock_guard<std::mutex> l(m_state);
				m_info = ii;

				return true;
			}
		}
		catch (Err::Exception& ex) {
			m_file.reset();

			Log << L"(AbstractCodec::LoadHeader) " << ex.Desc() << "\n";
			return false;
		}
		catch (...) {
			m_file.reset();

			Log << L"(AbstractCodec::LoadHeader) Unrecognized exception.\n";
			return false;
		}

		m_file.reset();
		return false;
	}

	AbstractCodec::AllocationStatus AbstractCodec::Allocate(const Geom::SizeInt& dimHint){
		if (dimHint == Geom::SizeInt(0, 0)) {
			return PerformAllocate();
		}

		return PerformAllocate(dimHint);
	}

	size_t AbstractCodec::EstimateMemory() {
		return PerformEstimateMemory();
	}

	AbstractCodec::LoadStatus AbstractCodec::LoadImageData(bool mayReset) {
		if (m_file == 0) {
			DO_THROW(Err::InvalidCall, TX("No file was set"));
		}
		if (m_file->IsOpen() == false) {
			DO_THROW(Err::InvalidCall, TX("File not open"));
		}

		LoadStatus l;
		try {
			l = PerformLoadImageData(m_file);
			m_isFinished = (l != LoadStatus::Aborted);
		}
		catch (Err::Exception& e) {
			Log << L"(AbstractCodec::LoadImageData) " << e.Desc() << "\n";
			l = LoadStatus::Finished;
			m_isFinished = true;
		}

		if (m_isFinished && m_file && mayReset) {
			m_file->Close();
		}

		return l;
	}

	bool AbstractCodec::IsFinished() {
		return m_isFinished;
	}

	Geom::SizeInt AbstractCodec::GetSize() const {
		std::lock_guard<std::mutex> l(m_state);
		return m_info.Dimensions;
	}

	void AbstractCodec::Terminate() {
		m_doAbort = true;
	}

	AbstractCodec::AbstractCodec():
		m_doAbort{ false },
		m_fs{ 0 },
		m_isFinished{ false }
	{
		m_info.Dimensions = Geom::SizeInt(0, 0);
		m_info.SurfaceFormat = Img::Format::Undefined;
	}

	AbstractCodec::~AbstractCodec() {}

	bool AbstractCodec::DoTerminate() {
		return m_doAbort;
	}

	size_t AbstractCodec::PerformEstimateMemory() {
		return Area(GetSize()) * EstimatePixelSize(GetFormat());
	}

	Img::Format AbstractCodec::GetFormat() const {
		std::lock_guard<std::mutex> l(m_state);
		return m_info.SurfaceFormat;
	}

	void AbstractCodec::ResetTerminate() {
		m_doAbort = false;
	}

	AbstractCodec::AllocationStatus AbstractCodec::PerformAllocate() {
		AllocationStatus a = PerformAllocate(GetSize());
		if (a == AllocationStatus::NotSupported) {
			return AllocationStatus::Failed;
		}
		return a;
	}

	AbstractCodec::AllocationStatus AbstractCodec::PerformAllocate( const Geom::SizeInt& ) {
		return AllocationStatus::NotSupported;
	}

	bool AbstractCodec::CanDetectFormat() const {
		return true;
	}
}
