#ifndef SURFACE_H
#define SURFACE_H

#include "surface_lock.h"
#include "types.h"

namespace Img {
	class Surface:boost::noncopyable {
	public:
		typedef std::shared_ptr<Surface> Ptr;

		class LockedArea {
		public:
			size_t Stride() const;
			uint8_t* Buffer();

			void Unlock();

			LockedArea(Surface* surface, uint8_t* buffer, const Geom::RectInt& rect, LockMethod isReadOnly, size_t stride);
			~LockedArea();

			typedef std::shared_ptr<LockedArea> Ptr;

		private:
			Geom::RectInt m_rectLocked;
			uint8_t* m_buffer;
			size_t m_stride;
			LockMethod m_method;
			Surface* m_surface;
			bool m_isLocked;
		};

	public:
		void CreateSurface(const Geom::SizeInt& size, Format format);
		void Deallocate();

		bool CopyRectToBuffer(uint8_t* pDest, int strideDest, const Geom::RectInt& sourceRect, const Img::Color& bgrColor, int brightness, int contrast, int gamma, bool applyAlpha);

		void ClearSurface(const Img::Color& col);

		void CopySurface(Surface::Ptr source);
		void CopySurface(Surface::Ptr source, Geom::PointInt destinationTopLeft);
		void CopySurface(Surface::Ptr source, Geom::RectInt sourceAreaToCopy, Geom::PointInt destinationTopLeft);

		void BlitSurfaceColorKey(Surface::Ptr source, Geom::PointInt destinationTopLeft, uint8_t colorKeyIndex);
		void BlitSurfaceColorKey(Surface::Ptr source, Geom::RectInt sourceAreaToCopy, Geom::PointInt destinationTopLeft, uint8_t colorKeyIndex);

		void FillRect(const Geom::RectInt& areaToFill, const Img::Color& colorToSet);

		bool IsDirty();

		LockedArea::Ptr LockSurface(LockMethod method = LockReadWrite);
		LockedArea::Ptr LockSurface(const Geom::RectInt& region, LockMethod method = LockReadWrite);

		Img::Palette GetPalette();
		void SetPalette(const Img::Palette& pal);

		int PixelSize() const;
		const Geom::SizeInt& GetSize() const;

		int Width() const { return m_size.Width; }
		int Height() const { return m_size.Height; }

		Format GetFormat() const;

		Surface();
		virtual ~Surface();

	protected:
		void ForceDirty();
		void SetLockStrategy(LockStrategy::Ptr strategy);
		void UnlockSurface(LockMethod method);

		friend class LockedArea;

	private:
		virtual void onDeallocate()=0;
		virtual bool onCreateSurface()=0;
		virtual void onClearSurface(const Img::Color& col);
		virtual void onCopySurface(Surface::Ptr source, Geom::RectInt sourceAreaToCopy, Geom::PointInt destinationTopLeft);
		virtual void onFillRect(const Geom::RectInt& areaToFill, const Color& colorToSet);
		virtual void onBlitSurfaceColorKey(Surface::Ptr source, Geom::RectInt sourceAreaToCopy, Geom::PointInt destinationTopLeft, uint8_t colorKeyIndex);

		void copyNoConversion( Surface::Ptr source, Geom::RectInt &sourceAreaToCopy, Geom::PointInt destinationTopLeft );
		void copyConvertAnyTo32( Surface::Ptr source, Geom::RectInt &sourceAreaToCopy, Geom::PointInt destinationTopLeft );

		virtual uint8_t* onLockSurface(const Geom::RectInt& region, LockMethod method = LockReadWrite)=0;
		virtual void onUnlockSurface()=0;

		virtual void onPaletteUpdate()=0;

		virtual size_t onStride() const = 0;

		virtual void onCopyAreaToBuffer(uint8_t* pDest, int strideDest, const Geom::RectInt& sourceRect, const Img::Color& bgrColor, int brightness, int contrast, int gamma, bool applyAlpha);

	private:
		LockStrategy::Ptr m_strategyLocking;

		std::mutex m_mutexDirty;

		int m_pixelSize;
		Geom::SizeInt m_size;
		bool m_isDirty;
		Format m_swFormat;
		bool m_isCreated;

		Img::Palette m_swPal;
	};

	struct FilterBufferAndLock {
		Filter::FilterBuffer filterBuffer;
		Surface::LockedArea::Ptr lock;
	};
	
	FilterBufferAndLock GenerateFilterBuffer(Img::Surface::Ptr src);

	_Check_return_ Geom::SizeInt CalculateUnzoomedSize(_In_opt_ std::shared_ptr<Surface> surf, _In_ Filter::RotationAngle angle);
}

#endif
